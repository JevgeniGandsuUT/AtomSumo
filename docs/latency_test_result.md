# HTTP Latency Test Result

## Test context

- Date: `2026-04-11`
- Base URL: `http://192.168.4.1`
- Network: `Robot-Control`
- Controller status: confirmed alive over USB serial on `COM5`
- Safety condition: `speed=0` was set before the test so that HTTP command latency could be measured without uncontrolled movement
- Latest validated run: `2026-04-11 19:34:31`

## Test method

- `120` HTTP requests were sent in total
- commands alternated between:
  - `/move?dir=forward`
  - `/move?dir=stop`
- response time was measured from request start until full HTTP response reception

Artifacts:

- latest CSV data: [../analysis/results/http_latency_2026-04-11_19-34-31.csv](../analysis/results/http_latency_2026-04-11_19-34-31.csv)
- latest summary: [../analysis/results/http_latency_2026-04-11_19-34-31_summary.txt](../analysis/results/http_latency_2026-04-11_19-34-31_summary.txt)
- previous CSV data: [../analysis/results/http_latency_2026-04-11_18-28-33.csv](../analysis/results/http_latency_2026-04-11_18-28-33.csv)
- previous summary: [../analysis/results/http_latency_2026-04-11_18-28-33_summary.txt](../analysis/results/http_latency_2026-04-11_18-28-33_summary.txt)
- script: [../analysis/http_latency_test.py](../analysis/http_latency_test.py)

## Result summary

Latest run:

- sample count: `120`
- median latency: `31.538 ms`
- p95 latency: `53.042 ms`
- max latency: `68.883 ms`

Per endpoint:

- `/move?dir=forward`: `n=60`, median `30.623 ms`, p95 `53.042 ms`, max `68.883 ms`
- `/move?dir=stop`: `n=60`, median `32.673 ms`, p95 `50.741 ms`, max `67.651 ms`

Comparison to previous run:

- previous overall median: `127.809 ms`
- latest overall median: `31.538 ms`
- improvement: about `75%` lower median latency
- previous `forward` requests were much slower than `stop`
- latest run shows `forward` and `stop` are now closely matched

## Medium-speed run

Additional run at medium motor speed:

- timestamp: `2026-04-11 19:38:18`
- configured speed value: `180`
- CSV data: [../analysis/results/http_latency_2026-04-11_19-38-18_medium_speed.csv](../analysis/results/http_latency_2026-04-11_19-38-18_medium_speed.csv)
- summary: [../analysis/results/http_latency_2026-04-11_19-38-18_medium_speed_summary.txt](../analysis/results/http_latency_2026-04-11_19-38-18_medium_speed_summary.txt)

Overall:

- sample count: `120`
- median latency: `87.373 ms`
- p95 latency: `158.018 ms`
- max latency: `186.038 ms`

Per endpoint:

- `/move?dir=forward`: `n=60`, median `147.644 ms`, p95 `167.233 ms`, max `186.038 ms`
- `/move?dir=stop`: `n=60`, median `32.405 ms`, p95 `39.185 ms`, max `44.610 ms`

Comparison against `speed=0` run:

- `speed=0` overall median: `31.538 ms`
- `speed=180` overall median: `87.373 ms`
- the increase comes almost entirely from `/move?dir=forward`
- `stop` remains fast and stable even when the motors are enabled at a realistic speed setting

## Interpretation

- The robot control path is reachable and repeatable over Wi-Fi.
- The latest HTTP control latency now comfortably meets the V1 target of median `<= 100 ms` and p95 `<= 150 ms`.
- The previous asymmetry between `forward` and `stop` has disappeared, which is consistent with the wiring issue on motor channel `A` having been resolved.
- In operator terms, this latency should feel responsive and should not be a bottleneck for browser-based manual control.
- The result still measures HTTP request-response time, not physical wheel start time, so it is best described as control-path latency rather than full motion latency.
- At `speed=180`, the system still remains in a usable range for manual control, but the `forward` command now sits close to or slightly above the stricter p95 target from the V1 specification.

## Remaining limitation

This test measures HTTP command latency only. It does not yet replace the physical motor test, because no direct measurement of:

- wheel speed,
- current draw,
- push force,
- or real braking distance

was taken in this run.
