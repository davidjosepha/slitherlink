#ifndef ROTATE_H
#define ROTATE_H

#include "coordinates.h"
#include "enums.h"

Coordinates rotateNumber(int i, int j, int m, int n, Orientation orient);
Coordinates rotateHLine(int i, int j, int m, int n, Orientation orient);
Coordinates rotateVLine(int i, int j, int m, int n, Orientation orient);

#endif
