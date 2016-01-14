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
        bool numberSatisfied(int i, int j) const;
        bool isSolved() const;
        void copy(Grid & newGrid);
        bool getValid() const { return valid_; };
        void setValid(bool validity) { valid_ = validity && valid_; };
        bool containsClosedContours() const;

    private:
        void mergeContours(Contour & newContour);
        bool valid_ = true;

        std::vector<Contour> contours_;
};

#endif