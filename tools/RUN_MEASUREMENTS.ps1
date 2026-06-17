$ErrorActionPreference = "Stop"

$RepoRoot = Split-Path -Parent (Split-Path -Parent $MyInvocation.MyCommand.Path)
Set-Location $RepoRoot

function Wait-Key {
    Write-Host ""
    Write-Host "Press Enter to close..." -ForegroundColor DarkGray
    [void][System.Console]::ReadLine()
}

function Test-Robot {
    Write-Host "Checking robot at http://192.168.4.1 ..." -ForegroundColor Cyan
    try {
        $response = Invoke-WebRequest -Uri "http://192.168.4.1" -UseBasicParsing -TimeoutSec 3
        Write-Host "Robot reachable: HTTP $($response.StatusCode)" -ForegroundColor Green
        return $true
    } catch {
        Write-Host "Robot is not reachable." -ForegroundColor Red
        Write-Host "Connect this laptop to Wi-Fi SSID: Robot-Control" -ForegroundColor Yellow
        Write-Host "Then run this script again." -ForegroundColor Yellow
        return $false
    }
}

function Run-Python {
    param(
        [Parameter(Mandatory = $true)]
        [string]$ScriptPath
    )

    $python = Get-Command python -ErrorAction SilentlyContinue
    if (-not $python) {
        $python = Get-Command py -ErrorAction SilentlyContinue
    }
    if (-not $python) {
        throw "Python was not found in PATH. Install Python or add it to PATH."
    }

    if ($python.Name -eq "py.exe" -or $python.Name -eq "py") {
        & $python.Source -3 $ScriptPath
    } else {
        & $python.Source $ScriptPath
    }
}

Write-Host "ATOM Sumo measurement runner" -ForegroundColor Cyan
Write-Host "Repository: $RepoRoot"
Write-Host ""
Write-Host "1. Power brownout test (automatic)"
Write-Host "2. ToF accuracy test (interactive)"
Write-Host "3. Run both in recommended order"
Write-Host ""

$choice = Read-Host "Choose 1, 2, or 3"

try {
    switch ($choice) {
        "1" {
            if (Test-Robot) {
                Run-Python "tests\scripts\power_brownout_test.py"
            }
        }
        "2" {
            if (Test-Robot) {
                Run-Python "tests\scripts\tof_accuracy_collect.py"
            }
        }
        "3" {
            if (Test-Robot) {
                Run-Python "tests\scripts\power_brownout_test.py"
                Run-Python "tests\scripts\tof_accuracy_collect.py"
            }
        }
        default {
            Write-Host "Unknown choice: $choice" -ForegroundColor Red
        }
    }

    Write-Host ""
    Write-Host "Done." -ForegroundColor Green
    Write-Host "Result files are in tests\results." -ForegroundColor Green
} catch {
    Write-Host ""
    Write-Host "Error:" -ForegroundColor Red
    Write-Host $_.Exception.Message -ForegroundColor Red
}

Wait-Key
