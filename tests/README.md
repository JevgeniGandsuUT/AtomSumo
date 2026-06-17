# Tests and Measurements

This folder contains executable test scripts, raw measurement data, and Jupyter notebooks.

## Scripts

- [HTTP latency test](scripts/http_latency_test.py)
- [Power brownout test](scripts/power_brownout_test.py)
- [ToF accuracy collector](scripts/tof_accuracy_collect.py)
- [Notebook generator](scripts/generate_test_results_notebook.py)

## Results

- [CSV and summary result files](results)
- [ToF calibration summary](results/tof_calibration_summary.csv)
- [Camera homography point template](results/camera_homography_points_template.csv)
- [Tap-drive accuracy template](results/tap_drive_accuracy_test_template.csv)

## Notebooks

- [HTTP latency analysis](notebooks/http_latency_analysis.ipynb)
- [Full test-results analysis](notebooks/test_results_analysis.ipynb)
- [Camera calibration and tap-drive notebook](notebooks/camera_calibration_tap_drive.ipynb)
- [Executed camera calibration and tap-drive notebook](notebooks/camera_calibration_tap_drive.executed.ipynb)

Run the full notebook generator from the repository root:

```powershell
python tests\scripts\generate_test_results_notebook.py
```
