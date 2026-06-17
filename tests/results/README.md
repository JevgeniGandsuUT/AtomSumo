# Test Results

This folder contains the CSV/TXT measurement files used by the test notebooks.

Current datasets:

- HTTP latency runs from 2026-04-11.
- HTTP latency runs from 2026-05-30 restored from the executed notebook data.
- Movement speed, drift, and turn tests restored from the executed notebook data.
- ToF stop and accuracy tests restored from the executed notebook data.
- Power brownout/reset test restored from the executed notebook data.
- Power brownout retest summary for the stable PWM `80-180` operating range.
- Tap-drive accuracy test with five measured physical target trials.
- Camera homography point set, JSON matrix, and residual-error table.
- Color repeatability test for red, light, and dark surfaces.
- Operator-observed auto behavior row; full logger CSV still pending.
- Camera homography and tap-drive templates for the next Verstapost 3 measurements.

The 2026-05-30 latency and physical test CSV files were restored after the OneDrive-backed `analysis/results` folder was lost during restructuring. They are kept here as the working CSV source for the project reports and notebooks.

Template files are intentionally not treated as measured data until filled with physical test rows and saved under the non-template result name.
