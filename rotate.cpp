#include "rotate.h"
#include <utility>
#include "enums.h"

/* Translates coordinates for a number on a lattice in a
 * given orientation to the coordinates for that number
 * in its canonical orientation and returns the new
 * adjusted coordinates. */
std::pair<int, int> rotateNumber(int i, int j, int m, int n, Orientation orient) {
    switch (orient) {
        case UPFLIP:
            i = m - i - 1;
        case UP:
            return std::pair<int, int> { i, j };
        case DOWNFLIP:
            i = m - i - 1;
        case DOWN:
            return std::pair<int, int> { m-i-1, n-j-1 };
        case LEFTFLIP:
            i = n - i - 1;
        case LEFT:
            return std::pair<int, int> { j, n-i-1 };
        case RIGHTFLIP:
            i = n - i - 1;
        case RIGHT:
            return std::pair<int, int> { m-j-1, i };
    }
}

/* Translates coordinates for a horizontal edge on a
 * lattice in a given orientation to the coordinates
 * for that edge in its canonical orientation and
 * returns the new adjusted coordinates. */
std::pair<int, int> rotateHLine(int i, int j, int m, int n, Orientation orient) {
    switch (orient) {
        case UPFLIP:
            i = m - i;
        case UP:
            return std::pair<int, int> { i, j };
        case DOWNFLIP:
            i = m - i;
        case DOWN:
            return std::pair<int, int> { m-i, n-j-1 };
        case LEFTFLIP:
            i = n - i;
        case LEFT:
            return std::pair<int, int> { j, n-i };
        case RIGHTFLIP:
            i = n - i;
        case RIGHT:
            return std::pair<int, int> { m-j-1, i };
    }
}

/* Translates coordinates for a vertical edge on a
 * lattice in a given orientation to the coordinates
 * for that edge in its canonical orientation and
 * returns the new adjusted coordinates. */
std::pair<int, int> rotateVLine(int i, int j, int m, int n, Orientation orient) {
    switch (orient) {
        case UPFLIP:
            i = m - i - 1;
        case UP:
            return std::pair<int, int> { i, j };
        case DOWNFLIP:
            i = m - i - 1;
        case DOWN:
            return std::pair<int, int> { m-i-1, n-j };
        case LEFTFLIP:
            i = n - i - 1;
        case LEFT:
            return std::pair<int, int> { j, n-i-1 };
        case RIGHTFLIP:
            i = n - i - 1;
        case RIGHT:
            return std::pair<int, int> { m-j, i };
    }
}
