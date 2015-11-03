#ifndef STRUCTS_H
#define STRUCTS_H

#include "enums.h"

struct Coordinates {
    int i;
    int j;
};

struct NumberPosition {
    Coordinates coords;
    Number num;
};

struct EdgePosition {
    Coordinates coords;
    Edge edge;
};

struct PrioEdge{
	Coordinates coords;
	double priority;
	bool h;
};

#endif
