#ifndef GRID_H
#define GRID_H
#include "lattice.h"
#include <vector>
#include "enums.h"
#include "contour.h"

class Grid : public Lattice {
    public:
        Grid() { };
        ~Grid();
        void initUpdateMatrix();
        int getUpdateMatrix(int i, int j);
        void setUpdateMatrix(int i, int j, bool b);
        int getContraMatrix(int i, int j);
        void setContraMatrix(int i, int j, bool b);
        virtual bool setHLine(int i, int j, Edge edge);
        virtual bool setVLine(int i, int j, Edge edge);
        virtual bool changeHLine(int i, int j, Edge edge);
        virtual bool changeVLine(int i, int j, Edge edge);
        bool numberSatisfied(int i, int j) const;
        bool isSolved() const;
        void copy(Grid & newGrid);
        bool getValid() const { return valid_; };
        void setValid(bool validity) { valid_ = validity && valid_; };
        void resetGrid();
        bool containsClosedContours() const;

    private:
        void mergeContours(Contour & newContour);
        bool ** updateMatrix_;
        bool ** contraMatrix_;
        bool valid_ = true;
        bool init_ = false;

        std::vector<Contour> contours_;
};

#endif
