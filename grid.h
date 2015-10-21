#ifndef GRID_H
#define GRID_H
#include "lattice.h"
#include <vector>
#include "enums.h"
#include "contour.h"

class Grid : public Lattice {
    public:
        Grid() { };
        virtual bool setHLine(int i, int j, Edge edge);
        virtual bool setVLine(int i, int j, Edge edge);
        bool numberSatisfied(int i, int j);
        bool isSolved();
        void gridCopy(Grid & newGrid);
        bool isValid() {return isValid_;};
        bool setIsValid(bool validity) { if (isValid_) {isValid_ = validity;}};

    private:
        void mergeContours(Contour & newContour);
        bool isValid_ = true;

        std::vector<Contour> contours_;
};

#endif
