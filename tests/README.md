# Tests and Measurements

This folder contains executable test scripts, raw measurement data, and Jupyter notebooks.

## Scripts

- [HTTP latency test](scripts/http_latency_test.py)
- [Power brownout test](scripts/power_brownout_test.py)
- [ToF accuracy collector](scripts/tof_accuracy_collect.py)
- [Notebook generator](scripts/generate_test_results_notebook.py)

## Results

- [CSV and summary result files](results)

## Notebooks

- [HTTP latency analysis](notebooks/http_latency_analysis.ipynb)
- [Full test-results analysis](notebooks/test_results_analysis.ipynb)

Run the full notebook generator from the repository root:

```powershell
python tests\scripts\generate_test_results_notebook.py
```
