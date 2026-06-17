$ErrorActionPreference = "Stop"

$RepoRoot = Split-Path -Parent (Split-Path -Parent $MyInvocation.MyCommand.Path)
Set-Location $RepoRoot

function Wait-Key {
    Write-Host ""
    Write-Host "Press Enter to close..." -ForegroundColor DarkGray
    [void][System.Console]::ReadLine()
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

Write-Host "ATOM Sumo power brownout test" -ForegroundColor Cyan
Write-Host "Connect this laptop to Wi-Fi SSID: Robot-Control before running." -ForegroundColor Yellow
Write-Host ""

try {
    Invoke-WebRequest -Uri "http://192.168.4.1" -UseBasicParsing -TimeoutSec 3 | Out-Null
    Run-Python "tests\scripts\power_brownout_test.py"
    Write-Host ""
    Write-Host "Done. Result: tests\results\power_brownout_test.csv" -ForegroundColor Green
} catch {
    Write-Host ""
    Write-Host "Could not run the test." -ForegroundColor Red
    Write-Host $_.Exception.Message -ForegroundColor Red
}

Wait-Key
