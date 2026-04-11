/*
 * AtomS3R-CAM brighter stream
 * Goal: brighter image first, with higher FPS and no obvious quality loss
 * on a sensor that does not support native JPEG output.
 */

#ifndef _M5_ATOM_S3R_CAM_H_
#define _M5_ATOM_S3R_CAM_H_

#define PWDN_GPIO_NUM  -1
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM  21
#define SIOD_GPIO_NUM  12
#define SIOC_GPIO_NUM  9
#define Y9_GPIO_NUM    13
#define Y8_GPIO_NUM    11
#define Y7_GPIO_NUM    17
#define Y6_GPIO_NUM    4
#define Y5_GPIO_NUM    48
#define Y4_GPIO_NUM    46
#define Y3_GPIO_NUM    42
#define Y2_GPIO_NUM    3
#define VSYNC_GPIO_NUM 10
#define HREF_GPIO_NUM  14
#define PCLK_GPIO_NUM  40
#define POWER_GPIO_NUM 18

#endif

#include <WiFi.h>
#include <cstring>
#include "esp_camera.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"

#define USE_ATOMS3R_CAM
#define STA_MODE

#ifndef CAMERA_SLOT
// Set to 1 for first camera board, 2 for second camera board.
#define CAMERA_SLOT 2
#endif

#if (CAMERA_SLOT != 1) && (CAMERA_SLOT != 2)
#error "CAMERA_SLOT must be 1 or 2"
#endif

const char* ssid = "Robot-Control";
const char* password = "robot123";
const char* CAMERA_NAME = (CAMERA_SLOT == 1) ? "cam1" : "cam2";
const uint8_t CAMERA_IP_LAST_OCTET = (CAMERA_SLOT == 1) ? 20 : 21;

WiFiServer server(80);

static const uint16_t STREAM_CHUNK_BYTES = 16 * 1024;
static const uint8_t JPEG_QUALITY_STREAM = 72;
static const uint8_t JPEG_QUALITY_CAPTURE = 82;
static const size_t HTTP_LINE_BYTES = 256;
static const size_t STREAM_JPEG_BUFFER_BYTES = 32 * 1024;
static const size_t STREAM_FRAME_BUFFER_BYTES = 64 * 1024;
static const framesize_t STREAM_FRAME_SIZE = FRAMESIZE_QVGA;
static const uint32_t REQUEST_START_TIMEOUT_MS = 1000;
static const uint32_t HTTP_LINE_TIMEOUT_MS = 1000;
static const uint8_t HTTP_MAX_HEADER_LINES = 40;
static const BaseType_t STREAM_CAPTURE_CORE = 0;

static volatile bool g_streamClientActive = false;
static uint8_t* g_streamJpegBuffer = nullptr;
static uint8_t* g_streamFrameLatest = nullptr;
static uint8_t* g_streamFrameWork = nullptr;
static uint8_t* g_streamFrameTx = nullptr;
static size_t g_streamFrameLatestLen = 0;
static uint32_t g_streamFrameSeq = 0;
static SemaphoreHandle_t g_streamFrameMutex = nullptr;
static TaskHandle_t g_streamCaptureTaskHandle = nullptr;

static camera_config_t camera_config = {
    .pin_pwdn      = PWDN_GPIO_NUM,
    .pin_reset     = RESET_GPIO_NUM,
    .pin_xclk      = XCLK_GPIO_NUM,
    .pin_sscb_sda  = SIOD_GPIO_NUM,
    .pin_sscb_scl  = SIOC_GPIO_NUM,
    .pin_d7        = Y9_GPIO_NUM,
    .pin_d6        = Y8_GPIO_NUM,
    .pin_d5        = Y7_GPIO_NUM,
    .pin_d4        = Y6_GPIO_NUM,
    .pin_d3        = Y5_GPIO_NUM,
    .pin_d2        = Y4_GPIO_NUM,
    .pin_d1        = Y3_GPIO_NUM,
    .pin_d0        = Y2_GPIO_NUM,
    .pin_vsync     = VSYNC_GPIO_NUM,
    .pin_href      = HREF_GPIO_NUM,
    .pin_pclk      = PCLK_GPIO_NUM,

    .xclk_freq_hz  = 20000000,
    .ledc_timer    = LEDC_TIMER_0,
    .ledc_channel  = LEDC_CHANNEL_0,

    .pixel_format  = PIXFORMAT_RGB565,
    .frame_size    = STREAM_FRAME_SIZE,
    .jpeg_quality  = 12,
    .fb_count      = 2,
    .fb_location   = CAMERA_FB_IN_PSRAM,
    .grab_mode     = CAMERA_GRAB_LATEST,
    .sccb_i2c_port = 0,
};

static void tuneCamera() {
    sensor_t* s = esp_camera_sensor_get();
    if (!s) return;

    s->set_framesize(s, STREAM_FRAME_SIZE);

    const uint16_t pid = s->id.PID;
    Serial.printf("Sensor PID: 0x%04X\n", pid);

    if (pid == GC0308_PID) {
        // GC0308 uses sensor-specific semantics for "contrast" and "gain_ctrl".
        // Force sane defaults and then apply a mild low-light boost.
        s->set_reg(s, 0xfe, 0xFF, 0x00); // page 0
        s->set_reg(s, 0xb3, 0xFF, 0x40); // neutral contrast (default)
        s->set_reg(s, 0x50, 0xFF, 0x28); // global gain (default 0x14)
        return;
    }

    // Generic profile for sensors that support these controls.
    s->set_brightness(s, 1);
    s->set_contrast(s, 0);
    s->set_saturation(s, 0);
    s->set_sharpness(s, 0);
    s->set_denoise(s, 1);
    s->set_whitebal(s, 1);
    s->set_awb_gain(s, 1);
    s->set_exposure_ctrl(s, 1);
    s->set_aec2(s, 1);
    s->set_ae_level(s, 1);
    s->set_gain_ctrl(s, 1);
    s->set_agc_gain(s, 20);
    s->set_quality(s, 10);
}

static bool writeClientFully(WiFiClient& client, const uint8_t* data, size_t len) {
    size_t sent = 0;
    while (sent < len && client.connected()) {
        size_t chunk = min((size_t)STREAM_CHUNK_BYTES, len - sent);
        size_t written = client.write(data + sent, chunk);
        if (written == 0) {
            return false;
        }
        sent += written;
    }
    return sent == len;
}

struct JpegClientContext {
    WiFiClient* client;
    uint8_t* buffer;
    size_t capacity;
    size_t buffered;
    bool ok;
};

static size_t jpgWriteToClient(void* arg, size_t index, const void* data, size_t len) {
    (void)index;

    JpegClientContext* ctx = static_cast<JpegClientContext*>(arg);
    if (!ctx || !ctx->client || !ctx->ok || !ctx->client->connected() || !ctx->buffer || ctx->capacity == 0) {
        if (ctx) {
            ctx->ok = false;
        }
        return 0;
    }

    const uint8_t* src = static_cast<const uint8_t*>(data);
    size_t remaining = len;
    while (remaining > 0 && ctx->ok) {
        size_t room = ctx->capacity - ctx->buffered;
        if (room == 0) {
            if (!writeClientFully(*ctx->client, ctx->buffer, ctx->buffered)) {
                ctx->ok = false;
                break;
            }
            ctx->buffered = 0;
            room = ctx->capacity;
        }
        size_t chunk = min(room, remaining);
        memcpy(ctx->buffer + ctx->buffered, src, chunk);
        ctx->buffered += chunk;
        src += chunk;
        remaining -= chunk;
    }

    return len - remaining;
}

struct FrameEncodeContext {
    uint8_t* buffer;
    size_t capacity;
    size_t len;
    bool ok;
};

static size_t jpgWriteToFrameBuffer(void* arg, size_t index, const void* data, size_t len) {
    (void)index;

    FrameEncodeContext* ctx = static_cast<FrameEncodeContext*>(arg);
    if (!ctx || !ctx->buffer || !ctx->ok) {
        return 0;
    }

    size_t room = (ctx->capacity > ctx->len) ? (ctx->capacity - ctx->len) : 0;
    size_t chunk = min(room, len);
    if (chunk > 0) {
        memcpy(ctx->buffer + ctx->len, data, chunk);
        ctx->len += chunk;
    }
    if (chunk < len) {
        ctx->ok = false;
    }
    return chunk;
}

static void streamCaptureTask(void* arg) {
    (void)arg;

    for (;;) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        while (g_streamClientActive) {
            if (!g_streamFrameWork) {
                vTaskDelay(pdMS_TO_TICKS(10));
                continue;
            }

            camera_fb_t* fb = esp_camera_fb_get();
            if (!fb) {
                vTaskDelay(pdMS_TO_TICKS(2));
                continue;
            }

            bool ok = false;
            size_t outLen = 0;

            if (fb->format == PIXFORMAT_JPEG) {
                if (fb->len <= STREAM_FRAME_BUFFER_BYTES) {
                    memcpy(g_streamFrameWork, fb->buf, fb->len);
                    outLen = fb->len;
                    ok = true;
                }
            } else {
                FrameEncodeContext ctx = {g_streamFrameWork, STREAM_FRAME_BUFFER_BYTES, 0, true};
                ok = frame2jpg_cb(fb, JPEG_QUALITY_STREAM, jpgWriteToFrameBuffer, &ctx) && ctx.ok;
                outLen = ctx.len;
            }

            esp_camera_fb_return(fb);

            if (!ok || outLen == 0) {
                vTaskDelay(pdMS_TO_TICKS(1));
                continue;
            }

            if (xSemaphoreTake(g_streamFrameMutex, pdMS_TO_TICKS(5)) == pdTRUE) {
                uint8_t* oldLatest = g_streamFrameLatest;
                g_streamFrameLatest = g_streamFrameWork;
                g_streamFrameWork = oldLatest;
                g_streamFrameLatestLen = outLen;
                g_streamFrameSeq++;
                xSemaphoreGive(g_streamFrameMutex);
            }

            taskYIELD();
        }
    }
}

static bool sendFrameAsJpeg(WiFiClient& client, camera_fb_t* fb, uint8_t quality) {
    if (!fb) {
        return false;
    }

    if (fb->format == PIXFORMAT_JPEG) {
        return writeClientFully(client, fb->buf, fb->len);
    }
    if (!g_streamJpegBuffer) {
        return false;
    }

    JpegClientContext ctx = {&client, g_streamJpegBuffer, STREAM_JPEG_BUFFER_BYTES, 0, true};
    bool converted = frame2jpg_cb(fb, quality, jpgWriteToClient, &ctx);
    if (converted && ctx.ok && ctx.buffered > 0) {
        if (!writeClientFully(*ctx.client, ctx.buffer, ctx.buffered)) {
            ctx.ok = false;
        }
        ctx.buffered = 0;
    }
    return converted && ctx.ok;
}

static bool readHttpLine(WiFiClient& client, char* line, size_t lineSize, uint32_t timeoutMs = HTTP_LINE_TIMEOUT_MS) {
    if (!line || lineSize == 0) {
        return false;
    }
    size_t len = 0;
    uint32_t started = millis();
    while (client.connected() && (millis() - started) < timeoutMs) {
        while (client.available()) {
            char c = (char)client.read();
            if (c == '\r') {
                continue;
            }
            if (c == '\n') {
                line[len] = '\0';
                return true;
            }
            if ((len + 1) < lineSize) {
                line[len++] = c;
            }
        }
        delay(0);
    }
    line[len] = '\0';
    return false;
}

static bool drainHeaders(WiFiClient& client) {
    char line[HTTP_LINE_BYTES];
    uint8_t lineCount = 0;
    while (client.connected() && lineCount < HTTP_MAX_HEADER_LINES) {
        if (!readHttpLine(client, line, sizeof(line))) {
            return false;
        }
        if (line[0] == '\0') {
            return true;
        }
        lineCount++;
    }
    return false;
}

static void getPath(const char* reqLine, char* path, size_t pathSize) {
    if (!path || pathSize == 0) {
        return;
    }

    path[0] = '\0';
    if (!reqLine) {
        strncpy(path, "/", pathSize - 1);
        path[pathSize - 1] = '\0';
        return;
    }

    const char* p1 = strchr(reqLine, ' ');
    if (!p1) {
        strncpy(path, "/", pathSize - 1);
        path[pathSize - 1] = '\0';
        return;
    }

    const char* start = p1 + 1;
    const char* p2 = strchr(start, ' ');
    size_t len = p2 ? (size_t)(p2 - start) : strlen(start);
    if (len >= pathSize) {
        len = pathSize - 1;
    }
    memcpy(path, start, len);
    path[len] = '\0';

    char* q = strchr(path, '?');
    if (q) {
        *q = '\0';
    }
    if (path[0] == '\0') {
        strncpy(path, "/", pathSize - 1);
        path[pathSize - 1] = '\0';
    }
}

#define PART_BOUNDARY "123456789000000000000987654321"
static const char* STREAM_CONTENT_TYPE = "multipart/x-mixed-replace;boundary=" PART_BOUNDARY;
static const char* STREAM_BOUNDARY = "--" PART_BOUNDARY "\r\n";
static const char* STREAM_PART = "Content-Type: image/jpeg\r\n\r\n";

static void sendIndex(WiFiClient& client) {
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html; charset=utf-8");
    client.println("Cache-Control: no-store, no-cache, must-revalidate, max-age=0");
    client.println("Pragma: no-cache");
    client.println("Connection: close");
    client.println();

    client.println(
      "<!doctype html>"
      "<html><head>"
      "<meta charset='utf-8'>"
      "<meta name='viewport' content='width=device-width,initial-scale=1'>"
      "<title>AtomS3R-CAM Bright</title>"
      "</head><body style='margin:0;padding:20px;background:#111;color:#eee;font-family:Arial,sans-serif'>"
      "<h2 style='margin-top:0'>AtomS3R-CAM Bright</h2>"
      "<img src='/stream' width='320' style='max-width:100%;border:1px solid #333;background:#000'>"
      "<div style='margin-top:12px'><a href='/capture' target='_blank' style='color:#6cf'>Open snapshot</a></div>"
      "</body></html>"
    );

    client.stop();
}

static void sendCapture(WiFiClient& client) {
    camera_fb_t* fb = esp_camera_fb_get();
    if (!fb) {
        client.println("HTTP/1.1 500 Internal Server Error");
        client.println("Content-Type: text/plain; charset=utf-8");
        client.println("Connection: close");
        client.println();
        client.println("Capture failed");
        client.stop();
        return;
    }

    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: image/jpeg");
    client.println("Cache-Control: no-store, no-cache, must-revalidate, max-age=0");
    client.println("Pragma: no-cache");
    client.println("Connection: close");
    client.println();

    if (!sendFrameAsJpeg(client, fb, JPEG_QUALITY_CAPTURE)) {
        Serial.println("JPEG conversion failed during capture");
    }

    esp_camera_fb_return(fb);
    client.stop();
}

static void sendStream(WiFiClient& client) {
    if (!g_streamCaptureTaskHandle) {
        client.println("HTTP/1.1 503 Service Unavailable");
        client.println("Content-Type: text/plain; charset=utf-8");
        client.println("Connection: close");
        client.println();
        client.println("Stream task unavailable");
        client.stop();
        return;
    }

    if (g_streamClientActive) {
        client.println("HTTP/1.1 503 Service Unavailable");
        client.println("Content-Type: text/plain; charset=utf-8");
        client.println("Connection: close");
        client.println();
        client.println("Stream is busy");
        client.stop();
        return;
    }

    g_streamClientActive = true;
    struct StreamGuard {
        ~StreamGuard() { g_streamClientActive = false; }
    } guard;

    if (g_streamCaptureTaskHandle) {
        xTaskNotifyGive(g_streamCaptureTaskHandle);
    }

    client.setNoDelay(true);

    client.println("HTTP/1.1 200 OK");
    client.printf("Content-Type: %s\r\n", STREAM_CONTENT_TYPE);
    client.println("Content-Disposition: inline; filename=capture.jpg");
    client.println("Access-Control-Allow-Origin: *");
    client.println("Cache-Control: no-store, no-cache, must-revalidate, max-age=0");
    client.println("Pragma: no-cache");
    client.println();

    uint32_t lastSentSeq = 0;

    while (client.connected()) {
        const uint8_t* framePtr = nullptr;
        size_t frameLen = 0;
        uint32_t frameSeq = 0;

        if (xSemaphoreTake(g_streamFrameMutex, pdMS_TO_TICKS(10)) == pdTRUE) {
            if (g_streamFrameSeq != lastSentSeq && g_streamFrameLatest && g_streamFrameLatestLen > 0 &&
                g_streamFrameLatestLen <= STREAM_FRAME_BUFFER_BYTES) {
                uint8_t* oldTx = g_streamFrameTx;
                g_streamFrameTx = g_streamFrameLatest;
                g_streamFrameLatest = oldTx;
                frameLen = g_streamFrameLatestLen;
                frameSeq = g_streamFrameSeq;
                framePtr = g_streamFrameTx;
            }
            xSemaphoreGive(g_streamFrameMutex);
        }

        if (!framePtr || frameLen == 0) {
            delay(1);
            continue;
        }

        client.print(STREAM_BOUNDARY);
        client.print(STREAM_PART);

        if (!writeClientFully(client, framePtr, frameLen)) {
            client.stop();
            return;
        }

        client.print("\r\n");
        lastSentSeq = frameSeq;

        delay(0);
    }

    client.stop();
}

static void connectWiFi() {
    IPAddress localIp(192, 168, 4, CAMERA_IP_LAST_OCTET);
    IPAddress gateway(192, 168, 4, 1);
    IPAddress subnet(255, 255, 255, 0);
    IPAddress dns(192, 168, 4, 1);

    WiFi.mode(WIFI_STA);
    WiFi.setHostname(CAMERA_NAME);
    WiFi.setSleep(false);
    WiFi.setTxPower(WIFI_POWER_19_5dBm);

    if (!WiFi.config(localIp, gateway, subnet, dns)) {
        Serial.println("Failed to set static IP config");
    }

    WiFi.begin(ssid, password);

    Serial.printf("Connecting %s to %s\n", CAMERA_NAME, ssid);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    Serial.printf("%s IP: %s\n", CAMERA_NAME, WiFi.localIP().toString().c_str());
    Serial.printf("%s stream: http://%s/stream\n", CAMERA_NAME, WiFi.localIP().toString().c_str());
}

void setup() {
    Serial.begin(115200);

    g_streamJpegBuffer = static_cast<uint8_t*>(ps_malloc(STREAM_JPEG_BUFFER_BYTES));
    g_streamFrameLatest = static_cast<uint8_t*>(ps_malloc(STREAM_FRAME_BUFFER_BYTES));
    g_streamFrameWork = static_cast<uint8_t*>(ps_malloc(STREAM_FRAME_BUFFER_BYTES));
    g_streamFrameTx = static_cast<uint8_t*>(ps_malloc(STREAM_FRAME_BUFFER_BYTES));
    g_streamFrameMutex = xSemaphoreCreateMutex();
    if (!g_streamJpegBuffer || !g_streamFrameLatest || !g_streamFrameWork || !g_streamFrameTx || !g_streamFrameMutex) {
        Serial.println("Failed to allocate stream JPEG buffer in PSRAM");
        delay(1000);
        esp_restart();
    }

    BaseType_t captureTaskCreated = xTaskCreatePinnedToCore(
        streamCaptureTask,
        "streamCaptureTask",
        8192,
        nullptr,
        2,
        &g_streamCaptureTaskHandle,
        STREAM_CAPTURE_CORE
    );
    if (captureTaskCreated != pdPASS || !g_streamCaptureTaskHandle) {
        Serial.println("Failed to start stream capture task");
        delay(1000);
        esp_restart();
    }

    pinMode(POWER_GPIO_NUM, OUTPUT);
    digitalWrite(POWER_GPIO_NUM, LOW);
    delay(500);

    esp_err_t err = esp_camera_init(&camera_config);
    if (err != ESP_OK) {
        Serial.printf("Camera Init Fail: 0x%x\n", err);
        delay(1000);
        esp_restart();
    }

    Serial.println("Camera Init Success");
    tuneCamera();
    connectWiFi();
    server.begin();
    server.setNoDelay(true);
}

void loop() {
    WiFiClient client = server.available();
    if (!client) return;

    uint32_t started = millis();
    while (client.connected() && !client.available() && (millis() - started) < REQUEST_START_TIMEOUT_MS) {
        delay(1);
    }

    if (!client.connected() || !client.available()) {
        client.stop();
        return;
    }

    char reqLine[HTTP_LINE_BYTES];
    char path[HTTP_LINE_BYTES];
    if (!readHttpLine(client, reqLine, sizeof(reqLine))) {
        client.stop();
        return;
    }

    if (!drainHeaders(client)) {
        client.stop();
        return;
    }
    getPath(reqLine, path, sizeof(path));

    if (strcmp(path, "/") == 0) {
        sendIndex(client);
    } else if (strcmp(path, "/stream") == 0) {
        sendStream(client);
    } else if (strcmp(path, "/capture") == 0) {
        sendCapture(client);
    } else {
        client.println("HTTP/1.1 404 Not Found");
        client.println("Content-Type: text/plain; charset=utf-8");
        client.println("Connection: close");
        client.println();
        client.println("404 Not Found");
        client.stop();
    }

    client.stop();
}
