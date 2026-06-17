// Multiview ComfyUI run using front + left + back + right.
// Heavy mesh: use F5 preview in OpenSCAD. Not watertight.
scale(77)
    translate([-0.006192, -0.001091, -0.008271])
        import("comfy_multiview_4view_00001.stl", convexity = 10);
