// Compact robot chassis (max 150x150 mm) for OpenSCAD
// Units: millimeters
//
// Coordinate system:
// X = front/back (+X front), Y = left/right, Z = up

$fn = 54;

render_variant = "assembled";
print_layout_gap = 12;
show_scoop_in_printable_layout = false;
show_front_sensor_mount_in_printable_layout = false;
show_down_camera_mount_in_printable_layout = true;

// ---- Overall limits ----
platform_len = 150;   // max 150
platform_wid = 108;   // narrowed so the lower ATOM keeps about 10 mm from the motors
hull_h = 34;
wall_t = 3;
floor_t = 3;
corner_r = 10;

// ---- Ground clearance target ----
ground_clearance_target = 10; // 10 mm minimum

// ---- Motor placement ----
motor_x = 56;
motor_z = 8;
shaft_d = 3.0;
rear_axle_x = -59;
rear_axle_z = motor_z;
rear_axle_d = 3.0;
rear_axle_support_t = 6.0;
rear_axle_support_w = 10.0;
rear_axle_support_h = 16.0;

// ---- Powerbank bay (requested: 100 x 65 x 28 mm) ----
pb_len = 100;
pb_wid = 65;
pb_h = 28;
pb_clear = 1.2;
pb_back_shift = 34; // move powerbank bay farther rear to free center/front area
pb_rail_t = 2.2;
pb_slot_len = pb_len + pb_clear;
pb_slot_wid = pb_wid + pb_clear;
pb_slot_h = pb_h + 1.0;
pb_slot_x = platform_len / 2 - wall_t - 5 - pb_slot_len / 2 - pb_back_shift;
usb_side = 1; // 1 = right side, -1 = left side
pb_rear_open_w = pb_slot_wid + 0.8;
pb_rear_open_h = pb_slot_h + 0.8;
pb_rear_open_z = floor_t + pb_slot_h / 2;
rear_axle_support_y = pb_slot_wid / 2 + pb_rail_t + rear_axle_support_w / 2;

// ---- PCB top bay (requested 60 x 60 mm) ----
pcb_inner = 60.5;
pcb_outer = 70;
pcb_frame_h = 8;
pcb_z = hull_h;
pcb_rear_gap = 2; // keep a small printable gap from the rear wall
pcb_center_x = -platform_len / 2 + wall_t + pcb_outer / 2 + pcb_rear_gap;
pcb_hole_spacing = 54;
pcb_standoff_h = 7;
pcb_standoff_d = 6;
pcb_hole_d = 2.2;

// ---- N20 drive cavities ----
n20_len = 26.5;
n20_wid = 12.2;
n20_h = 10.2;
n20_clear = 0.8;

// ---- Front guard / sensor areas ----
front_guard_h = 24;
front_guard_w = 74;
front_guard_visor_h = 4;
front_guard_visor_d = 3;
atom_open_z = hull_h + 9;      // center of ATOM socket
atom_cam_open_w = 20;          // enlarged front window for ATOM camera
atom_cam_open_h = 14;
atom_cam_open_y = 0;
atom_cam_open_z = atom_open_z - 2;
tof_open_w = 11;
tof_open_h = 7;
tof_gap = 5;                   // vertical gap between ATOM cam and ToF windows
tof_above_atom_cam = true;     // set false to place ToF lower
tof_side = 1;                  // 1 = right of camera, -1 = left of camera
tof_offset_y = 26;             // stronger lateral offset to avoid camera overlap
tof_open_y = tof_side * tof_offset_y;
tof_open_z = atom_cam_open_z +
  (tof_above_atom_cam ? 1 : -1) * (atom_cam_open_h / 2 + tof_open_h / 2 + tof_gap);
tof_plate_h = 12;

// ---- ATOM S3R insert box ----
atom_inner_w = 24.8;
atom_inner_h = 24.8;
atom_inner_d = 14.0;
atom_wall = 2.0;
atom_box_w = atom_inner_w + 2 * atom_wall;
atom_box_h = atom_inner_h + 2 * atom_wall;
atom_box_d = atom_inner_d + 2 * atom_wall;
atom_box_front_embed = 1.8;
atom_box_brace_t = 2.4;
atom_box_brace_w = atom_box_w - 10;
atom_box_brace_h = 10;
atom_box_brace_under_len = 7;
front_atom_shell_top_lip = 3.0;
front_atom_shell_open_w = atom_inner_w + 1.6;
front_atom_shell_open_h = front_guard_h - front_atom_shell_top_lip;
front_atom_shell_open_z = hull_h + front_atom_shell_open_h / 2;
atom_front_top_lip = 4.0;
atom_front_bottom_lip = 4.0;
atom_front_open_w = atom_inner_w + 1.0;
atom_front_open_h = atom_box_h - atom_front_top_lip - atom_front_bottom_lip;
atom_front_open_z = atom_open_z + (atom_front_bottom_lip - atom_front_top_lip) / 2;
atom_rear_open_w = atom_box_w - 2.0;
atom_rear_open_h = atom_box_h - 8.0;
atom_rear_open_z = atom_open_z - 1.0;
front_atom_box_x = platform_len / 2 - wall_t - atom_box_d / 2 + atom_box_front_embed;
front_atom_box_bottom_z = atom_open_z - atom_box_h / 2;
front_atom_box_front_x = front_atom_box_x + atom_box_d / 2;

// ---- Down-facing front camera ----
down_cam_open = 20;
down_cam_x = platform_len / 2 - 18; // move closer to the nose
down_atom_inner_w = 24.8;       // AtomS3R-CAM body is 24 x 24 x 13.5 mm
down_atom_inner_l = 24.8;
down_atom_inner_h = 14.2;
down_atom_wall = 2.0;
down_atom_box_w = down_atom_inner_w + 2 * down_atom_wall;
down_atom_box_l = down_atom_inner_l + 2 * down_atom_wall;
down_atom_box_h = down_atom_inner_h + 2 * down_atom_wall;
down_atom_rear_open_w = down_atom_box_w + 0.8;
down_atom_rear_open_h = down_atom_box_h + 0.8;
down_atom_rear_open_z = floor_t + down_atom_box_h / 2;
down_atom_box_center_z = floor_t + down_atom_box_h / 2;
down_atom_box_front_x = down_cam_x + down_atom_box_l / 2;

// ---- XL6009 mount (typical board ~43 x 21 mm) ----
xl_gap_from_pcb = 4.0;
xl_center_y = 0;
xl_support_top_z = floor_t + pb_slot_h;
xl_mount_z = xl_support_top_z + floor_t / 2;
xl_body_len = 43;
xl_body_wid = 21;
xl_body_h = 14;
xl_clear = 0.8;
xl_tray_wall = 2.0;
xl_tray_floor = 2.5;
xl_tray_inner_len = xl_body_len + xl_clear;
xl_tray_inner_wid = xl_body_wid + xl_clear;
xl_tray_inner_h = xl_body_h + 0.8;
xl_tray_outer_len = xl_tray_inner_len + 2 * xl_tray_wall;
xl_tray_outer_wid = xl_tray_inner_wid + 2 * xl_tray_wall;
xl_rail_h = 5;
xl_stop_h = 4;
xl_zip_slot_w = 3.2;
xl_zip_slot_len = 14;
xl_center_x = pcb_center_x + pcb_outer / 2 + xl_gap_from_pcb + xl_tray_outer_wid / 2;
xl_bridge_wid = pb_slot_wid + 2 * pb_rail_t;

// ---- N20 real mount blocks ----
n20_mount_wall = 2.2;
n20_mount_h = n20_h + 4;
n20_zip_slot_w = 3.2;
n20_zip_slot_h = 2.2;
n20_outer_x = n20_wid + 2 * n20_mount_wall + 1.2;
n20_outer_y = n20_len + 2 * n20_mount_wall;

// ---- Futuristic front kit ----
show_futuristic_kit = true;
scoop_side_margin = 4;
scoop_width = platform_wid - 2 * scoop_side_margin;
scoop_reach = 26;
scoop_attach_z = 8;
scoop_plate_t = 3;
scoop_ground_gap = 3;
scoop_tip_z = scoop_ground_gap - ground_clearance_target + scoop_plate_t / 2;
scoop_tip_width = 34;
scoop_tip_thickness = 2;
scoop_attach_x = platform_len / 2 - wall_t / 2;
scoop_tip_x = platform_len / 2 + scoop_reach;
scoop_tip_bottom_z = scoop_tip_z - scoop_tip_thickness / 2;
scoop_tip_shift_x = -scoop_tip_x;
scoop_mount_bar_t = 5;
scoop_mount_bar_w = 60;
scoop_mount_bar_h = 10;
scoop_bolt_y = 22;
scoop_bolt_d = 3.4;
scoop_bolt_head_d = 6.4;
scoop_bolt_head_h = 1.8;
front_wall_tilt = 6;
rear_wall_tilt = 5;
wall_slope_y_margin = 8;
wall_slope_top_narrow = 12;
wall_slope_strip_h = 8;
core_detail_h = 1.2;
core_detail_t = 1.4;
core_detail_z = floor_t + core_detail_h / 2;

front_sensor_ear_w = 12;
front_sensor_ear_h = 8;
front_sensor_bolt_y = 22.5;
front_sensor_bolt_d = 2.9;
front_sensor_head_d = 5.2;
front_sensor_head_h = 1.6;

down_camera_ear_x = 16;
down_camera_ear_w = 6;
down_camera_ear_t = 3;
down_camera_bolt_y = down_atom_box_w / 2 + down_camera_ear_w / 2;
down_camera_bolt_d = 2.9;
down_camera_head_d = 5.2;
down_camera_head_h = 1.6;

module rounded_rect_2d(l, w, r) {
  rr = min(r, min(l, w) / 2);
  hull() {
    for (x = [-l / 2 + rr, l / 2 - rr]) {
      for (y = [-w / 2 + rr, w / 2 - rr]) {
        translate([x, y]) circle(r = rr);
      }
    }
  }
}

module standoff(px, py, pz, h, d_outer, d_hole) {
  difference() {
    translate([px, py, pz]) cylinder(h = h, d = d_outer);
    translate([px, py, pz - 0.1]) cylinder(h = h + 0.2, d = d_hole);
  }
}

module through_hole_x(x_start, py, pz, len, d) {
  translate([x_start, py, pz])
    rotate([0, 90, 0]) cylinder(h = len, d = d);
}

module countersink_x_front(x_front, py, pz, head_h, hole_d, head_d) {
  translate([x_front - head_h, py, pz])
    rotate([0, 90, 0]) cylinder(h = head_h + 0.05, d1 = hole_d, d2 = head_d);
}

module through_hole_z(px, py, z_start, len, d) {
  translate([px, py, z_start]) cylinder(h = len, d = d);
}

module countersink_z_bottom(px, py, z_bottom, head_h, hole_d, head_d) {
  translate([px, py, z_bottom])
    cylinder(h = head_h + 0.05, d1 = head_d, d2 = hole_d);
}

module scoop_mount_interface_cuts() {
  if (show_futuristic_kit) {
    for (ypos = [-scoop_bolt_y, scoop_bolt_y]) {
      through_hole_x(platform_len / 2 - 14, ypos, scoop_attach_z, 16, scoop_bolt_d);
      countersink_x_front(platform_len / 2 + 0.2, ypos, scoop_attach_z, scoop_bolt_head_h, scoop_bolt_d, scoop_bolt_head_d);
    }
  }
}

module front_sensor_mount_interface_cuts() {
  for (side = [-1, 1]) {
    through_hole_x(platform_len / 2 - wall_t - 1, side * front_sensor_bolt_y, atom_open_z, wall_t + 2, front_sensor_bolt_d);
    countersink_x_front(platform_len / 2 + 0.2, side * front_sensor_bolt_y, atom_open_z, front_sensor_head_h, front_sensor_bolt_d, front_sensor_head_d);
  }
}

module down_camera_mount_interface_cuts() {
  for (side = [-1, 1]) {
    through_hole_z(down_cam_x, side * down_camera_bolt_y, -0.2, floor_t + 0.6, down_camera_bolt_d);
    countersink_z_bottom(down_cam_x, side * down_camera_bolt_y, -0.05, down_camera_head_h, down_camera_bolt_d, down_camera_head_d);
  }
}

module base_shell(
  include_front_guard = true,
  include_front_visor = true,
  open_front_guard_to_top = false,
  open_rear_loading_to_top = false,
  include_round_wall_holes = true
) {
  difference() {
    union() {
      linear_extrude(hull_h)
        rounded_rect_2d(platform_len, platform_wid, corner_r);

      // Front protection extension
      if (include_front_guard) {
        translate([platform_len / 2 - wall_t / 2, 0, hull_h + front_guard_h / 2])
          cube([wall_t, front_guard_w, front_guard_h], center = true);
      }

      // Slimmer, slightly raised visor leaves more room to insert the front ATOM.
      if (include_front_guard && include_front_visor) {
        translate([platform_len / 2 - front_guard_visor_d / 2, 0, hull_h + front_guard_h + front_guard_visor_h / 2 - 0.5])
          cube([front_guard_visor_d, front_guard_w - 10, front_guard_visor_h], center = true);
      }

      // Built-in sloped front and rear walls give the shell a more futuristic stance.
      if (show_futuristic_kit) {
        front_base_x = platform_len / 2 - wall_t / 2 - 0.6;
        front_top_x = front_base_x - front_wall_tilt;
        rear_base_x = -platform_len / 2 + wall_t / 2 + 0.6;
        rear_top_x = rear_base_x + rear_wall_tilt;
        wall_panel_w = platform_wid - 2 * wall_slope_y_margin;

        hull() {
          translate([front_base_x, 0, wall_slope_strip_h / 2])
            cube([wall_t + 1.2, wall_panel_w, wall_slope_strip_h], center = true);
          translate([front_top_x, 0, hull_h - wall_slope_strip_h / 2])
            cube([wall_t + 1.2, wall_panel_w - wall_slope_top_narrow, wall_slope_strip_h], center = true);
        }

        hull() {
          translate([rear_base_x, 0, wall_slope_strip_h / 2])
            cube([wall_t + 1.2, wall_panel_w, wall_slope_strip_h], center = true);
          translate([rear_top_x, 0, hull_h - wall_slope_strip_h / 2])
            cube([wall_t + 1.2, wall_panel_w - wall_slope_top_narrow, wall_slope_strip_h], center = true);
        }
      }
    }

    // Main inner cavity
    translate([0, 0, floor_t])
      linear_extrude(hull_h - floor_t + 0.2)
        rounded_rect_2d(
          platform_len - 2 * wall_t,
          platform_wid - 2 * wall_t,
          max(corner_r - wall_t, 1)
        );

    // Rear loading opening so the powerbank can slide in from the back.
    if (open_rear_loading_to_top) {
      // Printable variant: remove the top bridge over the battery opening.
      rear_open_bottom_z = pb_rear_open_z - pb_rear_open_h / 2;
      translate([
        -platform_len / 2 + wall_t / 2,
        0,
        rear_open_bottom_z + (hull_h - rear_open_bottom_z) / 2
      ])
        cube([wall_t + 1.2, pb_rear_open_w, hull_h - rear_open_bottom_z + 1.0], center = true);
    } else {
      translate([
        -platform_len / 2 + wall_t / 2,
        0,
        pb_rear_open_z
      ])
        cube([wall_t + 1.2, pb_rear_open_w, pb_rear_open_h], center = true);
    }

    // Larger front relief opening so the ATOM can slide into its socket.
    if (include_front_guard && open_front_guard_to_top) {
      // Printable variant: keep the front upright, but remove the top bridge
      // so the opening reaches the top edge and nothing hangs mid-air.
      translate([platform_len / 2 - wall_t / 2, atom_cam_open_y, hull_h + front_guard_h / 2])
        cube([wall_t + 1.0, front_atom_shell_open_w, front_guard_h + 1.0], center = true);
    } else {
      translate([platform_len / 2 - wall_t / 2, atom_cam_open_y, front_atom_shell_open_z])
        cube([wall_t + 1.0, front_atom_shell_open_w, front_atom_shell_open_h], center = true);
    }

    // Mounting cuts for the detachable scoop and front holder. In the
    // printable core we keep these solid and drill them later if needed.
    if (include_round_wall_holes) {
      scoop_mount_interface_cuts();
      front_sensor_mount_interface_cuts();
    }

    // Down-facing camera window in floor
    translate([down_cam_x, 0, floor_t / 2])
      cube([down_cam_open, down_cam_open, floor_t + 1], center = true);

    // Countersunk fasteners tie the lower camera holder to the floor.
    down_camera_mount_interface_cuts();

    // N20 motor shaft holes + body cavities
    for (side = [-1, 1]) {
      y_outer = side * (platform_wid / 2 - wall_t / 2);
      y_motor = side * (platform_wid / 2 - wall_t - n20_len / 2 + 0.8);

      // Shaft hole through side wall
      if (include_round_wall_holes) {
        translate([motor_x, y_outer, motor_z])
          rotate([90, 0, 0]) cylinder(h = wall_t + 1.0, d = shaft_d, center = true);
      }

      // Motor body cavity
      translate([motor_x, y_motor, motor_z])
        cube([n20_wid + n20_clear, n20_len + n20_clear, n20_h + n20_clear], center = true);
    }

    // Rear through-hole for a full-width axle carrying the second wheel pair.
    if (include_round_wall_holes) {
      translate([rear_axle_x, 0, rear_axle_z])
        rotate([90, 0, 0]) cylinder(h = platform_wid + 2, d = rear_axle_d, center = true);
    }

  }
}

module powerbank_bay_features(include_front_lip = true, include_side_rails = true) {
  rail_t = pb_rail_t;
  stop_t = 3;

  // Side rails guide the powerbank while it slides in from the rear.
  if (include_side_rails) {
    for (sy = [-1, 1]) {
      translate([
        pb_slot_x,
        sy * (pb_slot_wid / 2 + rail_t / 2),
        floor_t + pb_slot_h / 2
      ])
        cube([pb_slot_len, rail_t, pb_slot_h], center = true);
    }
  }

  if (include_front_lip) {
    // Front low lip stops the pack in the assembled view, but it can be
    // omitted from the support-free printable core.
    translate([
      pb_slot_x + pb_slot_len / 2 + stop_t / 2,
      0,
      floor_t + 4 / 2
    ])
      cube([stop_t, pb_slot_wid + 2 * rail_t, 4], center = true);
  }
}

module rear_axle_supports(include_axle_hole = true) {
  difference() {
    union() {
      // Side support posts add a second fixing point for the rear axle
      // while keeping the center clear for the powerbank bay.
      for (side = [-1, 1]) {
        translate([
          rear_axle_x,
          side * rear_axle_support_y,
          floor_t + rear_axle_support_h / 2
        ])
          cube([rear_axle_support_t, rear_axle_support_w, rear_axle_support_h], center = true);
      }
    }

    if (include_axle_hole) {
      translate([rear_axle_x, 0, rear_axle_z])
        rotate([90, 0, 0]) cylinder(h = platform_wid + 2, d = rear_axle_d, center = true);
    }
  }
}

module pcb_top_bay() {
  // 60x60 bay frame
  difference() {
    translate([pcb_center_x, 0, pcb_z + pcb_frame_h / 2])
      cube([pcb_outer, pcb_outer, pcb_frame_h], center = true);
    translate([pcb_center_x, 0, pcb_z + pcb_frame_h / 2])
      cube([pcb_inner, pcb_inner, pcb_frame_h + 0.4], center = true);
  }

  // PCB standoffs
  for (px = [-pcb_hole_spacing / 2, pcb_hole_spacing / 2]) {
    for (py = [-pcb_hole_spacing / 2, pcb_hole_spacing / 2]) {
      standoff(pcb_center_x + px, py, pcb_z, pcb_standoff_h, pcb_standoff_d, pcb_hole_d);
    }
  }
}

module front_sensor_mounts(
  open_front_access_to_top = false,
  remove_top_rim = false,
  remove_bottom_rim = false,
  remove_rear_rim = false,
  include_bolt_holes = true
) {
  // ATOM S3R insert box (socket style)
  difference() {
    union() {
      translate([front_atom_box_x, 0, atom_open_z])
        cube([atom_box_d, atom_box_w, atom_box_h], center = true);

      // Side ears spread the fastener load into the front wall columns.
      for (side = [-1, 1]) {
        hull() {
          translate([front_atom_box_x, side * (atom_box_w / 2 - 1), atom_open_z])
            cube([atom_box_d - 1, 2, front_sensor_ear_h], center = true);
          translate([front_atom_box_x, side * front_sensor_bolt_y, atom_open_z])
            cube([atom_box_d - 1, front_sensor_ear_w, front_sensor_ear_h], center = true);
        }
      }

    }

    // Inner cavity (insert from top/rear)
    translate([front_atom_box_x, 0, atom_open_z])
      cube([atom_inner_d + 1.2, atom_inner_w, atom_inner_h], center = true);

    // Printable variant: open the bed-facing side wide enough so no thin
    // inner bridge remains after the part is rotated window-down.
    if (remove_bottom_rim) {
      translate([front_atom_box_x, 0, front_atom_box_bottom_z + atom_wall / 2])
        cube([atom_box_d + 0.8, atom_box_w + 0.8, atom_wall + 0.8], center = true);
    }

    // Top opening for easier insertion
    translate([front_atom_box_x, 0, atom_open_z + atom_box_h / 2 - atom_wall / 2])
      cube([
        remove_top_rim ? atom_box_d + 0.8 : atom_box_d + 0.6,
        remove_top_rim ? atom_box_w + 0.8 : atom_inner_w + 0.6,
        atom_wall + 0.6
      ], center = true);

    // Enlarged rear cable/service opening for wiring and connector access.
    translate([front_atom_box_x - atom_box_d / 2, 0, remove_rear_rim ? atom_open_z : atom_rear_open_z])
      cube([
        atom_wall + 0.8,
        atom_rear_open_w,
        remove_rear_rim ? atom_box_h + 0.8 : atom_rear_open_h
      ], center = true);

    // Front access opening through the ATOM socket wall.
    if (open_front_access_to_top) {
      translate([
        front_atom_box_x + atom_box_d / 2 - atom_wall / 2,
        atom_cam_open_y,
        front_atom_box_bottom_z + atom_box_h / 2
      ])
        cube([atom_wall + 1.0, atom_front_open_w, atom_box_h + 0.6], center = true);
    } else {
      translate([front_atom_box_x + atom_box_d / 2 - atom_wall / 2, atom_cam_open_y, atom_front_open_z])
        cube([atom_wall + 1.0, atom_front_open_w, atom_front_open_h], center = true);
    }

    // Two through-bolts clamp the holder to the side columns of the front wall.
    if (include_bolt_holes) {
      for (side = [-1, 1]) {
        through_hole_x(front_atom_box_x - atom_box_d / 2 - 0.2, side * front_sensor_bolt_y, atom_open_z, atom_box_d + 0.4, front_sensor_bolt_d);
      }
    }
  }
}

module front_battle_scoop(include_bolt_holes = true) {
  difference() {
    union() {
      // Triangle-like battle wedge: wide at the hull, narrower at the tip.
      hull() {
        translate([scoop_attach_x, 0, scoop_attach_z])
          cube([scoop_plate_t, scoop_width, scoop_plate_t], center = true);
        translate([scoop_tip_x, 0, scoop_tip_z])
          cube([scoop_plate_t, scoop_tip_width, scoop_tip_thickness], center = true);
      }

      // A thicker rear bar spreads frontal load into the bolt area.
      translate([platform_len / 2 - scoop_mount_bar_t / 2, 0, scoop_attach_z])
        cube([scoop_mount_bar_t, scoop_mount_bar_w, scoop_mount_bar_h], center = true);
    }

    // Two countersunk M3 bolts clamp the scoop into the chassis nose.
    if (include_bolt_holes) {
      for (ypos = [-scoop_bolt_y, scoop_bolt_y]) {
        through_hole_x(platform_len / 2 - 16, ypos, scoop_attach_z, 18, scoop_bolt_d);
        countersink_x_front(platform_len / 2 + 0.01, ypos, scoop_attach_z, scoop_bolt_head_h, scoop_bolt_d, scoop_bolt_head_d);
      }
    }
  }
}

module down_camera_mount(remove_top_rim = false, remove_bottom_rim = false) {
  // Boxed socket for a full AtomS3R-CAM mounted inside and looking downward.
  difference() {
    union() {
      translate([down_cam_x, 0, floor_t + down_atom_box_h / 2])
        cube([down_atom_box_l, down_atom_box_w, down_atom_box_h], center = true);

      // Side ears rest on the floor and accept bolts from below.
      for (side = [-1, 1]) {
        translate([down_cam_x, side * down_camera_bolt_y, floor_t + down_camera_ear_t / 2])
          cube([down_camera_ear_x, down_camera_ear_w, down_camera_ear_t], center = true);
      }
    }

    // Internal cavity for AtomS3R-CAM body
    translate([down_cam_x, 0, floor_t + down_atom_wall + down_atom_inner_h / 2])
      cube([down_atom_inner_l, down_atom_inner_w, down_atom_inner_h], center = true);

    // Top insertion opening
    translate([down_cam_x, 0, floor_t + down_atom_box_h - down_atom_wall / 2])
      cube([
        remove_top_rim ? down_atom_box_l + 0.8 : down_atom_inner_l + 0.8,
        remove_top_rim ? down_atom_box_w + 0.8 : down_atom_inner_w + 0.8,
        down_atom_wall + 0.8
      ], center = true);

    // Remove the rear wall entirely so the lower ATOM can slide in freely.
    translate([down_cam_x - down_atom_box_l / 2 + down_atom_wall / 2, 0, down_atom_rear_open_z])
      cube([down_atom_wall + 0.8, down_atom_rear_open_w, down_atom_rear_open_h], center = true);

    // Large optical window through the floor. In the printable variant this
    // opens wider so no small inner bridge remains over the window.
    translate([down_cam_x, 0, (floor_t + down_atom_wall) / 2])
      cube([
        remove_bottom_rim ? down_atom_inner_l + 0.8 : down_cam_open,
        remove_bottom_rim ? down_atom_inner_w + 0.8 : down_cam_open,
        floor_t + down_atom_wall + 1
      ], center = true);

    // Two M2.5 bolts from the underside secure the lower holder.
    for (side = [-1, 1]) {
      through_hole_z(down_cam_x, side * down_camera_bolt_y, floor_t - 0.1, down_camera_ear_t + 0.3, down_camera_bolt_d);
    }
  }
}

module xl6009_mount() {
  tray_x = xl_tray_outer_wid;
  tray_y = xl_tray_outer_len;
  inner_x = xl_tray_inner_wid;
  inner_y = xl_tray_inner_len;
  z_base = xl_mount_z;
  z_rail = xl_mount_z + floor_t / 2 + xl_rail_h / 2;
  z_stop = xl_mount_z + floor_t / 2 + xl_stop_h / 2;

  difference() {
      union() {
        // Rotated bridge plate rests directly on the powerbank bay walls.
        translate([xl_center_x, xl_center_y, z_base])
          cube([tray_x, xl_bridge_wid, floor_t], center = true);

        // Two long side rails
        for (sy = [-1, 1]) {
          translate([
            xl_center_x,
            xl_center_y + sy * (inner_y / 2 + xl_tray_wall / 2),
            z_rail
          ])
            cube([tray_x, xl_tray_wall, xl_rail_h], center = true);
        }

        // Rear stop and small front lip to keep the board in place
        translate([
          xl_center_x - inner_x / 2 - xl_tray_wall / 2,
          xl_center_y,
          z_stop
        ])
          cube([xl_tray_wall, tray_y, xl_stop_h], center = true);

        translate([
          xl_center_x + inner_x / 2 + xl_tray_wall / 2,
          xl_center_y,
          z_base
        ])
          cube([xl_tray_wall, tray_y, 3], center = true);
      }

      // Zip-tie slots through the tray floor for easy fixing.
      for (sx = [-12, 12]) {
        translate([xl_center_x + sx, xl_center_y, z_base])
          cube([xl_zip_slot_w, xl_zip_slot_len, floor_t + 0.8], center = true);
      }
  }
}

module n20_motor_mounts(simple_walls_only = false, remove_inner_walls = false) {
  for (side = [-1, 1]) {
    y_motor = side * (platform_wid / 2 - wall_t - n20_len / 2 + 0.8);
    outer_wall_y = y_motor + side * (n20_outer_y / 2 - n20_mount_wall / 2);
    inner_wall_y = y_motor - side * (n20_outer_y / 2 - n20_mount_wall / 2);
    front_wall_x = motor_x + n20_outer_x / 2 - n20_mount_wall / 2;
    rear_wall_x = motor_x - n20_outer_x / 2 + n20_mount_wall / 2;
    mount_wall_z = floor_t + n20_mount_h / 2;

    if (simple_walls_only) {
      // Printable variant: keep only simple support-free walls around the
      // motor cavity, with the inner wall omitted near the central ATOM.
      translate([motor_x, outer_wall_y, mount_wall_z])
        cube([n20_outer_x, n20_mount_wall, n20_mount_h], center = true);

      translate([front_wall_x, y_motor, mount_wall_z])
        cube([n20_mount_wall, n20_outer_y, n20_mount_h], center = true);

      translate([rear_wall_x, y_motor, mount_wall_z])
        cube([n20_mount_wall, n20_outer_y, n20_mount_h], center = true);

      if (!remove_inner_walls) {
        translate([motor_x, inner_wall_y, mount_wall_z])
          cube([n20_outer_x, n20_mount_wall, n20_mount_h], center = true);
      }
    } else {
      insert_slot_bottom_z = motor_z + (n20_h + 0.8) / 2 - 0.4;
      insert_slot_top_z = floor_t + n20_mount_h + 0.6;
      insert_slot_h = insert_slot_top_z - insert_slot_bottom_z;
      insert_slot_z = insert_slot_bottom_z + insert_slot_h / 2;

      difference() {
        // Cradle body attached to floor and partially to side wall
        translate([motor_x, y_motor, floor_t + n20_mount_h / 2])
          cube([n20_outer_x, n20_outer_y, n20_mount_h], center = true);

        // Motor body cavity
        translate([motor_x, y_motor, motor_z])
          cube([n20_wid + n20_clear, n20_len + n20_clear, n20_h + 0.8], center = true);

        // Full top slot so the N20 motor can drop in vertically from above.
        translate([motor_x, y_motor, insert_slot_z])
          cube([n20_wid + n20_clear + 1.6, n20_len + n20_clear + 1.6, insert_slot_h], center = true);

        // Shaft relief on outer side
        translate([
          motor_x,
          outer_wall_y,
          motor_z
        ])
          rotate([90, 0, 0]) cylinder(h = n20_mount_wall + 1.0, d = 7.0, center = true);

        // Cable notch on inner side
        translate([
          motor_x,
          inner_wall_y,
          motor_z
        ])
          cube([8, n20_mount_wall + 1.0, 6], center = true);

        // Zip-tie slots
        for (yoff = [-8, 8]) {
          translate([motor_x, y_motor + yoff, floor_t + 2.4])
            cube([n20_outer_x + 0.8, n20_zip_slot_w, n20_zip_slot_h], center = true);
        }
      }
    }
  }
}

module futuristic_core_details() {
  // Low raised panels on the floor add a sci-fi shell look without creating
  // overhangs or interfering with the battery bay.
  for (side = [-1, 1]) {
    hull() {
      translate([26, side * 31, core_detail_z])
        cube([36, core_detail_t, core_detail_h], center = true);
      translate([53, side * 22, core_detail_z])
        cube([18, core_detail_t, core_detail_h], center = true);
    }

    hull() {
      translate([-54, side * 43, core_detail_z])
        cube([22, core_detail_t, core_detail_h], center = true);
      translate([-26, side * 37, core_detail_z])
        cube([18, core_detail_t, core_detail_h], center = true);
    }

    // Exterior side armor ribs; all vertical faces print cleanly with the
    // chassis flat on the bed.
    for (xpos = [-42, -14, 18, 48]) {
      translate([xpos, side * (platform_wid / 2 - 0.7), 18])
        cube([5, core_detail_t, 22], center = true);
    }
  }

  // Subtle center spine for visual direction and extra floor stiffness.
  hull() {
    translate([-28, 0, core_detail_z])
      cube([24, core_detail_t, core_detail_h], center = true);
    translate([34, 0, core_detail_z])
      cube([48, core_detail_t, core_detail_h], center = true);
  }

  // Nose cheek plates make the front read less boxy while staying inside
  // the 150 mm envelope.
  for (side = [-1, 1]) {
    translate([70, side * 28, 15])
      cube([4, core_detail_t, 24], center = true);
    translate([61, side * 36, 11])
      cube([4, core_detail_t, 16], center = true);
  }
}

module chassis_body() {
  union() {
    base_shell();
    n20_motor_mounts();
    powerbank_bay_features();
    rear_axle_supports();
    front_sensor_mounts();
    down_camera_mount();
    if (show_futuristic_kit) {
      front_battle_scoop();
    }
  }
}

module printable_chassis_core() {
  union() {
    base_shell(true, false, true, true, false);
    futuristic_core_details();
    n20_motor_mounts(true, true);
    powerbank_bay_features(false, false);
    rear_axle_supports(false);
  }
}

module printable_scoop_part() {
  // Flip the scoop onto its rear mounting bar so it prints support-free.
  translate([scoop_tip_x + scoop_plate_t / 2, 0, scoop_attach_z + scoop_mount_bar_h / 2])
    rotate([0, 180, 0])
      front_battle_scoop(false);
}

module printable_front_sensor_mount_part() {
  // Put the front access window downward onto the bed so the cavity stays
  // support-free and no top bridge is left over the opening. Shift it so
  // the printable footprint stays centered near the origin.
  translate([-atom_open_z, 0, front_atom_box_front_x])
    rotate([0, 90, 0])
      front_sensor_mounts(true, true, false, true);
}

module printable_down_camera_mount_part() {
  // Put the camera window downward onto the bed, matching the front M5 box
  // printable orientation style. Flip it 180 degrees on the bed so the
  // larger mounting ears face the opposite direction in the layout.
  rotate([0, 0, 180])
    translate([-down_cam_x, 0, -floor_t])
      down_camera_mount(true, true);
}

module printable_layout(layout_gap = 16) {
  mount_row_y = -(platform_wid / 2 + layout_gap + max(
    max(atom_box_w / 2, front_sensor_bolt_y + front_sensor_ear_w / 2),
    max(down_atom_box_w / 2, down_camera_bolt_y + down_camera_ear_w / 2)
  ));
  front_mount_layout_x = -(atom_box_h / 2 + layout_gap / 2);
  down_mount_layout_x = down_atom_box_l / 2 + layout_gap / 2;

  printable_chassis_core();

  if (show_futuristic_kit && show_scoop_in_printable_layout) {
    translate([0, platform_wid / 2 + layout_gap + 29, 0])
      rotate([0, 0, 90])
        printable_scoop_part();
  }

  if (show_front_sensor_mount_in_printable_layout) {
    translate([front_mount_layout_x, mount_row_y, 0])
      printable_front_sensor_mount_part();
  }

  if (show_down_camera_mount_in_printable_layout) {
    translate([down_mount_layout_x, mount_row_y, 0])
      printable_down_camera_mount_part();
  }
}

module render_chassis_variant(variant = "assembled", layout_gap = 16) {
  if (variant == "printable_layout") {
    printable_layout(layout_gap);
  } else if (variant == "printable_core") {
    printable_chassis_core();
  } else if (variant == "printable_scoop") {
    printable_scoop_part();
  } else if (variant == "printable_front_sensor_mount") {
    printable_front_sensor_mount_part();
  } else if (variant == "printable_down_camera_mount") {
    printable_down_camera_mount_part();
  } else {
    chassis_body();
  }
}

render_chassis_variant(render_variant, print_layout_gap);
