// Compact robot chassis (max 150x150 mm) for OpenSCAD
// Units: millimeters
//
// Coordinate system:
// X = front/back (+X front), Y = left/right, Z = up

$fn = 54;

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
front_wall_tilt = 6;
rear_wall_tilt = 5;
wall_slope_y_margin = 8;
wall_slope_top_narrow = 12;
wall_slope_strip_h = 8;

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

module base_shell() {
  difference() {
    union() {
      linear_extrude(hull_h)
        rounded_rect_2d(platform_len, platform_wid, corner_r);

      // Front protection extension
      translate([platform_len / 2 - wall_t / 2, 0, hull_h + front_guard_h / 2])
        cube([wall_t, front_guard_w, front_guard_h], center = true);

      // Slimmer, slightly raised visor leaves more room to insert the front ATOM.
      translate([platform_len / 2 - front_guard_visor_d / 2, 0, hull_h + front_guard_h + front_guard_visor_h / 2 - 0.5])
        cube([front_guard_visor_d, front_guard_w - 10, front_guard_visor_h], center = true);

      // Built-in sloped front and rear walls give the shell a more futuristic stance.
      if (show_futuristic_kit) {
        front_base_x = platform_len / 2 - wall_t / 2 + 0.6;
        front_top_x = front_base_x - front_wall_tilt;
        rear_base_x = -platform_len / 2 + wall_t / 2 - 0.6;
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
    translate([
      -platform_len / 2 + wall_t / 2,
      0,
      pb_rear_open_z
    ])
      cube([wall_t + 1.2, pb_rear_open_w, pb_rear_open_h], center = true);

    // Larger front relief opening so the ATOM can slide into its socket.
    translate([platform_len / 2 - wall_t / 2, atom_cam_open_y, front_atom_shell_open_z])
      cube([wall_t + 1.0, front_atom_shell_open_w, front_atom_shell_open_h], center = true);

    // Down-facing camera window in floor
    translate([down_cam_x, 0, floor_t / 2])
      cube([down_cam_open, down_cam_open, floor_t + 1], center = true);

    // N20 motor shaft holes + body cavities
    for (side = [-1, 1]) {
      y_outer = side * (platform_wid / 2 - wall_t / 2);
      y_motor = side * (platform_wid / 2 - wall_t - n20_len / 2 + 0.8);

      // Shaft hole through side wall
      translate([motor_x, y_outer, motor_z])
        rotate([90, 0, 0]) cylinder(h = wall_t + 1.0, d = shaft_d, center = true);

      // Motor body cavity
      translate([motor_x, y_motor, motor_z])
        cube([n20_wid + n20_clear, n20_len + n20_clear, n20_h + n20_clear], center = true);
    }

    // Rear through-hole for a full-width axle carrying the second wheel pair.
    translate([rear_axle_x, 0, rear_axle_z])
      rotate([90, 0, 0]) cylinder(h = platform_wid + 2, d = rear_axle_d, center = true);

  }
}

module powerbank_bay_features() {
  rail_t = pb_rail_t;
  stop_t = 3;

  // Side rails guide the powerbank while it slides in from the rear.
  for (sy = [-1, 1]) {
    translate([
      pb_slot_x,
      sy * (pb_slot_wid / 2 + rail_t / 2),
      floor_t + pb_slot_h / 2
    ])
      cube([pb_slot_len, rail_t, pb_slot_h], center = true);
  }

  // Front low lip
  translate([
    pb_slot_x + pb_slot_len / 2 + stop_t / 2,
    0,
    floor_t + 4 / 2
  ])
    cube([stop_t, pb_slot_wid + 2 * rail_t, 4], center = true);
}

module rear_axle_supports() {
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

    translate([rear_axle_x, 0, rear_axle_z])
      rotate([90, 0, 0]) cylinder(h = platform_wid + 2, d = rear_axle_d, center = true);
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

module front_sensor_mounts() {
  // ATOM S3R insert box (socket style)
  atom_box_x = platform_len / 2 - wall_t - atom_box_d / 2 + atom_box_front_embed;
  atom_box_bottom_z = atom_open_z - atom_box_h / 2;
  difference() {
    union() {
      translate([atom_box_x, 0, atom_open_z])
        cube([atom_box_d, atom_box_w, atom_box_h], center = true);

      // Structural brace ties the front ATOM socket into the front wall so it
      // does not appear to float above the chassis.
      hull() {
        translate([platform_len / 2 - wall_t / 2 - atom_box_brace_t / 2, 0, hull_h + atom_box_brace_h / 2 - 1])
          cube([atom_box_brace_t, atom_box_brace_w, atom_box_brace_h], center = true);
        translate([atom_box_x - atom_box_d / 2 + atom_box_brace_under_len / 2 + 1, 0, atom_box_bottom_z + atom_box_brace_t / 2])
          cube([atom_box_brace_under_len, atom_box_brace_w - 2, atom_box_brace_t], center = true);
      }
    }

    // Inner cavity (insert from top/rear)
    translate([atom_box_x, 0, atom_open_z])
      cube([atom_inner_d + 1.2, atom_inner_w, atom_inner_h], center = true);

    // Top opening for easier insertion
    translate([atom_box_x, 0, atom_open_z + atom_box_h / 2 - atom_wall / 2])
      cube([atom_box_d + 0.6, atom_inner_w + 0.6, atom_wall + 0.6], center = true);

    // Enlarged rear cable/service opening for wiring and connector access.
    translate([atom_box_x - atom_box_d / 2, 0, atom_rear_open_z])
      cube([atom_wall + 0.8, atom_rear_open_w, atom_rear_open_h], center = true);

    // Front access opening through the ATOM socket wall.
    translate([atom_box_x + atom_box_d / 2 - atom_wall / 2, atom_cam_open_y, atom_front_open_z])
      cube([atom_wall + 1.0, atom_front_open_w, atom_front_open_h], center = true);
  }
}

module front_battle_scoop() {
  attach_x = platform_len / 2 - wall_t / 2;
  tip_x = platform_len / 2 + scoop_reach;

  // Triangle-like battle wedge: wide at the hull, narrower at the tip.
  hull() {
    translate([attach_x, 0, scoop_attach_z])
      cube([scoop_plate_t, scoop_width, scoop_plate_t], center = true);
    translate([tip_x, 0, scoop_tip_z])
      cube([scoop_plate_t, scoop_tip_width, scoop_tip_thickness], center = true);
  }
}

module down_camera_mount() {
  // Boxed socket for a full AtomS3R-CAM mounted inside and looking downward.
  difference() {
    translate([down_cam_x, 0, floor_t + down_atom_box_h / 2])
      cube([down_atom_box_l, down_atom_box_w, down_atom_box_h], center = true);

    // Internal cavity for AtomS3R-CAM body
    translate([down_cam_x, 0, floor_t + down_atom_wall + down_atom_inner_h / 2])
      cube([down_atom_inner_l, down_atom_inner_w, down_atom_inner_h], center = true);

    // Top insertion opening
    translate([down_cam_x, 0, floor_t + down_atom_box_h - down_atom_wall / 2])
      cube([down_atom_inner_l + 0.8, down_atom_inner_w + 0.8, down_atom_wall + 0.8], center = true);

    // Remove the rear wall entirely so the lower ATOM can slide in freely.
    translate([down_cam_x - down_atom_box_l / 2 + down_atom_wall / 2, 0, down_atom_rear_open_z])
      cube([down_atom_wall + 0.8, down_atom_rear_open_w, down_atom_rear_open_h], center = true);

    // Large optical window through the floor
    translate([down_cam_x, 0, (floor_t + down_atom_wall) / 2])
      cube([down_cam_open, down_cam_open, floor_t + down_atom_wall + 1], center = true);
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

module n20_motor_mounts() {
  for (side = [-1, 1]) {
    y_motor = side * (platform_wid / 2 - wall_t - n20_len / 2 + 0.8);
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
        y_motor + side * (n20_outer_y / 2 - n20_mount_wall / 2),
        motor_z
      ])
        rotate([90, 0, 0]) cylinder(h = n20_mount_wall + 1.0, d = 7.0, center = true);

      // Cable notch on inner side
      translate([
        motor_x,
        y_motor - side * (n20_outer_y / 2 - n20_mount_wall / 2),
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

chassis_body();
