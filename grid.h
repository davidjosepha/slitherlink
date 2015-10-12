#ifndef GRID_H
#define GRID_H
#include "lattice.h"
#include <vector>
#include "enums.h"
#include "contour.h"

class Grid : public Lattice {
    public:
        Grid() { };
        virtual void setHLine(int i, int j, Edge edge);
        virtual void setVLine(int i, int j, Edge edge);

    private:
        void mergeContours(Contour & newContour);

        std::vector<Contour> contours_;
};

#endif
