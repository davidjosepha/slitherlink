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
        virtual bool setHLine(int i, int j, Edge edge);
        virtual bool setVLine(int i, int j, Edge edge);
        virtual bool changeHLine(int i, int j, Edge edge);
        virtual bool changeVLine(int i, int j, Edge edge);
        bool numberSatisfied(int i, int j) const;
        bool isSolved() const;
        void copy(Grid & newGrid);
        void clearAndCopy(Grid & newGrid);
        bool getValid() const { return valid_; };
        void setValid(bool validity) { valid_ = validity && valid_; };
        void resetGrid();
        bool containsClosedContours() const;
        bool getUpdateMatrix(int i, int j) { return updateMatrix_[i][j]; };
        bool getContraMatrix(int i, int j) { return contraMatrix_[i][j]; };
        void setUpdateMatrix(int i, int j, bool b) { updateMatrix_[i][j] = b; };
        void setContraMatrix(int i, int j, bool b) { contraMatrix_[i][j] = b; };

    private:
        void mergeContours(Contour & newContour);
        void updateContourMatrix(int i, int j, bool hline);
        bool ** updateMatrix_;
        bool ** contraMatrix_;
        std::pair<int,int> ** contourMatrix_;
        bool valid_ = true;
        bool init_ = false;
        int numOpenLoops_;
        int numClosedLoops_;

        std::pair<int,int> getContourMatrix(int i, int j) { return contourMatrix_[i][j]; };
        void setContourMatrix(int i, int j, std::pair<int,int> p) { contourMatrix_[i][j] = p; };
};

#endif
