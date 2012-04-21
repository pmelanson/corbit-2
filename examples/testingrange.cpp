int main (int argc, char *argv[]) {
    VECTOR outs[NUM_VERTS];
    MATRIX m;
    int i, col;

    ship.minx = SCREEN_W;
    ship.miny = SCREEN_H;
    ship.maxx = ship.maxy = 0;

    get_rotation_matrix (&m, ship.rx, ship.ry, ship.rz);
    apply_matrix (&m, ship.aim.x, ship.aim.y, ship.aim.z, &outs[0].x, &outs[0].y, &outs[0].z);
    direction.x = outs[0].x;
    direction.y = outs[0].y;
    direction.z = outs[0].z;

    for (i = 0; i < NUM_FACES; i++)
        apply_matrix (&m, ship.faces[i].normal.x, ship.faces[i].normal.y, ship.faces[i].normal.z, &ship.faces[i].rnormal.x, &ship.faces[i].rnormal.y, &ship.faces[i].rnormal.z);

    get_transformation_matrix (&m, itofix (1), ship.rx, ship.ry, ship.rz, ship.x, ship.y, ship.z);

    for (i = 0; i < NUM_VERTS; i++) {
        apply_matrix (&m, ship.points[i].x, ship.points[i].y, ship.points[i].z, &outs[i].x, &outs[i].y, &outs[i].z);
        persp_project (outs[i].x, outs[i].y, outs[i].z, &outs[i].x, &outs[i].y);
        if (fixtoi (outs[i].x) < ship.minx)
            ship.minx = fixtoi (outs[i].x);
        if (fixtoi (outs[i].x) > ship.maxx)
            ship.maxx = fixtoi (outs[i].x);
        if (fixtoi (outs[i].y) < ship.miny)
            ship.miny = fixtoi (outs[i].y);
        if (fixtoi (outs[i].y) > ship.maxy)
            ship.maxy = fixtoi (outs[i].y);
    }

    for (i = 0; i < NUM_FACES; i++) {
        if (fixtof (ship.faces[i].rnormal.z) < 0.0) {
            col = fixtoi (fixmul (dot_product (ship.faces[i].rnormal.x, ship.faces[i].rnormal.y, ship.faces[i].rnormal.z, 0, 0, itofix (1) ), itofix (ship.faces[i].range) ) );
            if (col < 0)
                col = -col + ship.faces[i].colour;
            else
                col = col + ship.faces[i].colour;

            triangle (buffer, fixtoi (outs[ship.faces[i].v1].x), fixtoi (outs[ship.faces[i].v1].y),
            fixtoi (outs[ship.faces[i].v2].x), fixtoi (outs[ship.faces[i].v2].y),
            fixtoi (outs[ship.faces[i].v3].x), fixtoi (outs[ship.faces[i].v3].y),
            palette_color[col]);
        }
    }
}
END_OF_MAIN();
