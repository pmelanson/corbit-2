#include <allegro.h>

int main (int argc, char *argv[])
{
    // length and angle of the racing car's velocity vector
    fixed angle = itofix (0);
    fixed length = itofix (0);
    // x- and y-coordinates of the velocity vector
    fixed vel_x, vel_y;

    // x- and y-position of the racing car
    fixed x = itofix (SCREEN_W / 2);
    fixed y = itofix (SCREEN_H / 2);

    while (!key[KEY_ESC])
    {
        // erase the old image
        circlefill (screen, fixtoi(x), fixtoi(y), 10, makecol (0, 0, 0));

        // check the keys and move the car
        if (key[KEY_UP] && length < itofix (2))
            length += ftofix (0.005);
        if (key[KEY_DOWN] && length > itofix (0))
            length -= ftofix (0.005);
        if (key[KEY_LEFT])
            angle = (angle - itofix (1)) & 0xFFFFFF;
        if (key[KEY_RIGHT])
            angle = (angle + itofix (1)) & 0xFFFFFF;

        // calculate the x- and y-coordinates of the velocity vector
        vel_x = fmul (length, fcos (angle));
        vel_y = fmul (length, fsin (angle));

        // move the car, and make sure it stays within the screen
        x += vel_x;
        if (x >= itofix (SCREEN_W)) x -= itofix(SCREEN_W);
        if (x < itofix (0)) x += itofix(SCREEN_W);
        y += vel_y;
        if (y >= itofix (SCREEN_H)) y -= itofix(SCREEN_H);
        if (y < itofix (0)) y += itofix(SCREEN_H);

        // draw the racing car
        circle (screen, fixtoi(x), fixtoi(y), 10, makecol (0, 0, 255));
        line (screen, fixtoi(x), fixtoi(y),
            fixtoi (x + 9 * fcos (angle)),
            fixtoi (y + 9 * fsin (angle)),
            makecol (255, 0, 0));

        // wait for 10 milliseconds, or else we'd go too fast
        rest (10);
    }

    return (0);
}
