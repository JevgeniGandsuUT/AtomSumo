// Printable support-free layout for the ATOM Sumo chassis.
//
// Default view:
// - main chassis core without suspended mounts;
// - down-facing camera holder as a separate part.
//
// The front camera/ATOM holder is exported separately because PrusaSlicer
// correctly recommends supports for that part.
//
// If you want to export just one STL, change the first argument below to:
// "printable_core"
// "printable_scoop"
// "printable_front_sensor_mount"
// "printable_down_camera_mount"

use <chassis_n20.scad>;

render_chassis_variant("printable_layout", 12);
