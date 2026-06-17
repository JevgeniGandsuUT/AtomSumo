// Four quadrant cuts of the multiview mesh.
// Use F5 preview. These are open/non-watertight reference parts.

scale_factor = 77;

module part(file, offset, col) {
    color(col)
        translate(offset)
            scale(scale_factor)
                translate([-0.006192, -0.001091, -0.008271])
                    import(file, convexity = 10);
}

part("front_left_quadrant.stl", [-70, 70, 0], "dodgerblue");
part("front_right_quadrant.stl", [-70, -70, 0], "cyan");
part("rear_left_quadrant.stl", [70, 70, 0], "orange");
part("rear_right_quadrant.stl", [70, -70, 0], "tomato");
