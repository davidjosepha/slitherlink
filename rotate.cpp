#include "rotate.h"
#include "coordinates.h"
#include "enums.h"

/* Translates coordinates for a number on a lattice in a
 * given orientation to the coordinates for that number
 * in its canonical orientation and returns the new
 * adjusted coordinates. */
Coordinates rotateNumber(int i, int j, int m, int n, Orientation orient) {
    switch (orient) {
        case UPFLIP:
            i = m - i - 1;
        case UP:
            return Coordinates { i, j };
        case DOWNFLIP:
            i = m - i - 1;
        case DOWN:
            return Coordinates { m-i-1, n-j-1 };
        case LEFTFLIP:
            i = m - i - 1;
        case LEFT:
            return Coordinates { n-j-1, i };
        case RIGHTFLIP:
            i = m - i - 1;
        case RIGHT:
            return Coordinates { j, m-i-1 };
    }
}

/* Translates coordinates for a horizontal edge on a
 * lattice in a given orientation to the coordinates
 * for that edge in its canonical orientation and
 * returns the new adjusted coordinates. */
Coordinates rotateHLine(int i, int j, int m, int n, Orientation orient) {
    return rotateNumber(i, j, m+1, n, orient);
}

/* Translates coordinates for a vertical edge on a
 * lattice in a given orientation to the coordinates
 * for that edge in its canonical orientation and
 * returns the new adjusted coordinates. */
Coordinates rotateVLine(int i, int j, int m, int n, Orientation orient) {
    return rotateNumber(i, j, m, n+1, orient);
}
