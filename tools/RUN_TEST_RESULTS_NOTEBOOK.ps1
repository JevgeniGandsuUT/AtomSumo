$ErrorActionPreference = "Stop"

$repoRoot = Split-Path -Parent (Split-Path -Parent $MyInvocation.MyCommand.Path)
Set-Location $repoRoot

Write-Host "Executing ATOM Sumo test-results notebook..." -ForegroundColor Cyan
python tests\scripts\generate_test_results_notebook.py

Write-Host ""
Write-Host "Done: tests\notebooks\test_results_analysis.ipynb" -ForegroundColor Green
Write-Host "Open it with Jupyter Notebook / VS Code to inspect tables and graphs."
Write-Host ""
Read-Host "Press Enter to close"
