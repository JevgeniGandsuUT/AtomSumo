// Overview of split mesh parts.
// Use F5 preview. These imported meshes are open/non-watertight reference parts.

scale_factor = 77;

module part(file, offset, col) {
    color(col)
        translate(offset)
            scale(scale_factor)
                translate([-0.006192, -0.001091, -0.008271])
                    import(file, convexity = 10);
}

part("front_half.stl", [-90, 0, 0], "dodgerblue");
part("rear_half.stl", [90, 0, 0], "orange");
part("left_half.stl", [0, 70, 0], "limegreen");
part("right_half.stl", [0, -70, 0], "tomato");
