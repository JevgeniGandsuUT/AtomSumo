Mesh split notes
================

Source: `../comfy_multiview_4view_00001.stl`

Generated files:

- `clean_main.stl` / `clean_main.obj` - largest connected mesh component with small dust removed.
- `front_half.stl`, `rear_half.stl`, `left_half.stl`, `right_half.stl` - rough open spatial cuts.
- `front_left_quadrant.stl`, `front_right_quadrant.stl`, `rear_left_quadrant.stl`, `rear_right_quadrant.stl` - rough open quadrant cuts.
- `loose_component_*.stl` - larger loose fragments from the original mesh.

Important: these pieces are not closed printable solids. The source AI mesh is not watertight,
so these are useful for inspection, reference, cleanup, or manual CAD reconstruction.
