#ifndef ROTATE_H
#define ROTATE_H

#include <utility>
#include "enums.h"

std::pair<int, int> rotateNumber(int i, int j, int m, int n, Orientation orient);
std::pair<int, int> rotateHLine(int i, int j, int m, int n, Orientation orient);
std::pair<int, int> rotateVLine(int i, int j, int m, int n, Orientation orient);

#endif
