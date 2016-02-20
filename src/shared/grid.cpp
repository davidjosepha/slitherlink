 #include "grid.h"
#include <cassert>
#include <vector>
#include "contour.h"
#include "enums.h"

Grid::~Grid() {
    if (init_) {
        for (int i = 0; i < m_; i++) {
            delete [] updateMatrix_[i];
        }
        delete [] updateMatrix_;

        for (int i = 0; i < m_; i++) {
            delete [] contraMatrix_[i];
        }
        delete [] contraMatrix_;

        for (int i = 0; i < m_+1; i++) {
            delete [] contourMatrix_[i];
        }
        delete [] contourMatrix_;
    }
}

void Grid::resetGrid() {
    for (int i = 1; i < getHeight(); i++) {
        for (int j = 1; j < getWidth()-1; j++) {
            hlines_[i][j] = EMPTY;
        }
    }

    for (int i = 1; i < getHeight()-1; i++) {
        for (int j = 1; j < getWidth(); j++) {
            vlines_[i][j] = EMPTY;
        }
    }

    for (int i = 0; i < getHeight(); i++) {
        for (int j = 0; j < getWidth(); j++) {
            setUpdateMatrix(i, j, true);
            setContraMatrix(i, j, true);
        }
    }

    for (int i = 0; i < getHeight()+1; i++) {
        for (int j = 0; j < getWidth()+1; j++) {
            setContourMatrix(i, j, std::make_pair(-1,-1));
        }
    }

    numClosedLoops_ = 0;
    numOpenLoops_ = 0;
}

/*
 * Copies grid for the purpose of making a guess.
 */
void Grid::copy(Grid & newGrid) const {
    newGrid.initArrays(getHeight(), getWidth());
    newGrid.initUpdateMatrix();

    for (int i = 0; i < getHeight()+1; i++) {
        for (int j = 0; j < getWidth(); j++) {
            newGrid.changeHLine(i, j, getHLine(i,j));
        }
    }

    for (int i = 0; i < getHeight(); i++) {
        for (int j = 0; j < getWidth()+1; j++) {
            newGrid.changeVLine(i, j, getVLine(i,j));
        }
    }

    for (int i = 0; i < getHeight(); i++) {
        for (int j = 0; j < getWidth(); j++) {
            newGrid.setNumber(i, j, getNumber(i,j));
            newGrid.setUpdateMatrix(i, j, updateMatrix_[i][j]);
            newGrid.setContraMatrix(i, j, contraMatrix_[i][j]);
        }
    }

    for (int i = 0; i < getHeight()+1; i++) {
        for (int j = 0; j < getWidth()+1; j++) {
            newGrid.setContourMatrix(i, j, contourMatrix_[i][j]);
        }
    }

    newGrid.numOpenLoops_ = numOpenLoops_;
    newGrid.numClosedLoops_ = numClosedLoops_;
}


void Grid::clearAndCopy(Grid & newGrid) {
    for (int i = 0; i < getHeight()+1; i++) {
        for (int j = 0; j < getWidth(); j++) {
            newGrid.changeHLine(i, j, getHLine(i,j));
        }
    }

    for (int i = 0; i < getHeight(); i++) {
        for (int j = 0; j < getWidth()+1; j++) {
            newGrid.changeVLine(i, j, getVLine(i,j));
        }
    }

    for (int i = 0; i < getHeight(); i++) {
        for (int j = 0; j < getWidth(); j++) {
            newGrid.setNumber(i, j, getNumber(i,j));
            newGrid.setUpdateMatrix(i, j, updateMatrix_[i][j]);
            newGrid.setContraMatrix(i, j, contraMatrix_[i][j]);
        }
    }

    for (int i = 0; i < getHeight()+1; i++) {
        for (int j = 0; j < getWidth()+1; j++) {
            newGrid.setContourMatrix(i, j, contourMatrix_[i][j]);
        }
    }

}

/*;
 * Set a horizontal line to a given edge type, checking to see if this change overwrites
 * any previous value set to that position. If a line is added, create a new contour and
 * attempt to merge that contour to any adjacent contours.
 */
bool Grid::setHLine(int i, int j, Edge edge) {
    assert(0 <= i && i < m_+1 && 0 <= j && j < n_);

    if (edge == EMPTY) {
        return true;
    }

    Edge prevEdge = getHLine(i, j);
    if (prevEdge == EMPTY) {
        hlines_[i][j] = edge;
    } else if (prevEdge != edge) {
        return false;
    } else if (prevEdge == edge) {
        return true;
    }

    // Update contour information
    if (edge == LINE) {
        updateContourMatrix(i, j, true);
    }

    // Update which parts of grid have possible rules that could be applied
    for (int x = std::max(0, i-3); x < std::min(i+1, getHeight()); x++) {
        for (int y = std::max(0, j-2); y < std::min(j+1, getWidth()); y++) {
            updateMatrix_[x][y] = true;
        }
    }

    // Update which parts of grid have possible contradictions
    for (int x = std::max(0, i-2); x < std::min(i+1, getHeight()); x++) {
        for (int y = std::max(0, j-1); y < std::min(j+1, getWidth()); y++) {
            contraMatrix_[x][y] = true;
        }
    }

    return true;
}

/*
 * Set a vertical line to a given edge type, checking to see if this change overwrites
 * any previous value set to that position. If a line is added, create a new contour and
 * attempt to merge that contour to any adjacent contours.
 */
bool Grid::setVLine(int i, int j, Edge edge) {
    assert(0 <= i && i < m_ && 0 <= j && j < n_+1);

    Edge prevEdge = getVLine(i, j);
    if (prevEdge == EMPTY) {
        vlines_[i][j] = edge;
    } else if (prevEdge != edge) {
        return false;
    } else if (prevEdge == edge) {
        return true;
    }
    
    // Update contour information
    if (edge == LINE) {
        updateContourMatrix(i, j, false);
    }

    // Update which parts of grid have possible rules that could be applied
    for (int x = std::max(0, i-2); x < std::min(i+1, getHeight()); x++) {
        for (int y = std::max(0, j-3); y < std::min(j+1, getWidth()); y++) {
            updateMatrix_[x][y] = true;
        }
    }

    // Update which parts of grid have possible contradictions
    for (int x = std::max(0, i-1); x < std::min(i+1, getHeight()); x++) {
        for (int y = std::max(0, j-2); y < std::min(j+1, getWidth()); y++) {
            contraMatrix_[x][y] = true;
        }
    }

    return true;
}

/*
 * Set a horizontal line to a given edge type, but allows overwrites.
 * Intended for the purpose of puzzle creation.
 */
bool Grid::changeHLine(int i, int j, Edge edge) {
    assert(0 <= i && i < m_+1 && 0 <= j && j < n_);

    hlines_[i][j] = edge;


    return true;
}

/*
 * Set a vertical line to a given edge type, but allows overwrites.
 * Intended for the purpose of puzzle creation.
 */
bool Grid::changeVLine(int i, int j, Edge edge) {
    assert(0 <= i && i < m_ && 0 <= j && j < n_+1);

    vlines_[i][j] = edge;

    return true;
}

/*
 * Check whether a given number has been satisfied with the proper number of lines
 * surrounding it.
 */
bool Grid::numberSatisfied(int i, int j) const {
    assert(0 <= i && i < m_ && 0 <= j && j < n_);

    Number number = numbers_[i][j];

    /* determine number of lines around number */
    int numLines = (hlines_[i][j] == LINE)
                 + (hlines_[i+1][j] == LINE)
                 + (vlines_[i][j] == LINE)
                 + (vlines_[i][j+1] == LINE);

    switch (number) {
        case NONE:
            return true;
        case ZERO:
            return numLines == 0;
        case ONE:
            return numLines == 1;
        case TWO:
            return numLines == 2;
        case THREE:
            return numLines == 3;
    }
}

/*
 * Checks if the puzzle is solved by assuring that there is only one contour
 * and that each number has been satisfied
 */
bool Grid::isSolved() const {
    if (numOpenLoops_ != 0 || numClosedLoops_ != 1) {
        return false;
    }

    for (int i = 0; i < m_; i++) {
        for (int j = 0; j < n_; j++) {
            if (!numberSatisfied(i,j)) {
                return false;
            }
        }
    }

    return true;
}

/*
 * Checks if there are any closed contours with the intention of detecting
 * prematurely closed contours
 */
bool Grid::containsClosedContours() const {
    return (numClosedLoops_>0);
}

void Grid::initUpdateMatrix() {

    if (!init_) {
        updateMatrix_ = new bool*[m_];
        for (int i = 0; i < getHeight(); i++) {
            updateMatrix_[i] = new bool[n_];
            for (int j = 0; j < getWidth(); j++) {
                updateMatrix_[i][j] = true;
            }
        }
        contraMatrix_ = new bool*[m_];
        for (int i = 0; i < getHeight(); i++) {
            contraMatrix_[i] = new bool[n_];
            for (int j = 0; j < getWidth(); j++) {
                contraMatrix_[i][j] = false;
            }
        }
        contourMatrix_ = new std::pair<int,int>*[m_+1];
        for (int i = 0; i < getHeight()+1; i++) {
            contourMatrix_[i] = new std::pair<int,int>[n_+1];
            for (int j = 0; j < getWidth()+1; j++) {
                contourMatrix_[i][j] = std::make_pair<int,int>(-1,-1);
            }
        }
        numOpenLoops_ = 0;
        numClosedLoops_ = 0;
    }

    init_ = true;
};

/* 
 * Updates the information on the endpoints of our contours according to what
 * new line has been added. We use this 2D array to keep track of the endpoints now 
 * instead of a vector. Also keeps track of the current number of open
 * and closed loops in our grid
 */
void Grid::updateContourMatrix(int i, int j, bool hline) {

    int i2 = i;
    int j2 = j;

    // The second endpoint of the new line is determined by whether the line is
    // horizontal or vertical
    if (hline) {
        j2 = j + 1;
    } else {
        i2 = i + 1;
    }

    /* Both ends of the new line are already endpoints to a single contour.
       So get rid of both open endpoints and add one count of a closed loop. */
    if (getContourMatrix(i,j).first == i2 && getContourMatrix(i,j).second == j2 && getContourMatrix(i2,j2).first == i && getContourMatrix(i2,j2).second == j) {
        setContourMatrix(i,j,std::make_pair(-1,-1));
        setContourMatrix(i2,j2,std::make_pair(-1,-1));
        numClosedLoops_++;
        numOpenLoops_--;
    } 
    /* Both ends of the new line are already endpoints to two different
     * conoturs. Get rid of the open endpoints, update the new ends of the
     * merged contour, and count one less open contour */
    else if (getContourMatrix(i,j).first != -1 && getContourMatrix(i2,j2).first != -1) {
        setContourMatrix(getContourMatrix(i,j).first,getContourMatrix(i,j).second,getContourMatrix(i2,j2));
        setContourMatrix(getContourMatrix(i2,j2).first,getContourMatrix(i2,j2).second,getContourMatrix(i,j));
        setContourMatrix(i,j,std::make_pair(-1,-1));
        setContourMatrix(i2,j2,std::make_pair(-1,-1));
        numOpenLoops_--;
    } 
    /* First end of the new line is already an endpoint to a contour. Extend
     * the contour and update new endpoints. */
    else if (getContourMatrix(i,j).first != -1) {
        setContourMatrix(getContourMatrix(i,j).first,getContourMatrix(i,j).second,std::make_pair(i2,j2));
        setContourMatrix(i2,j2,getContourMatrix(i,j));
        setContourMatrix(i,j,std::make_pair(-1,-1));
    } 
    /* Second end of the new line is already an endpoint to a contour. Extend
     * the contour and update new endpoints. */
    else if (getContourMatrix(i2,j2).first != -1) {
        setContourMatrix(getContourMatrix(i2,j2).first,getContourMatrix(i2,j2).second,std::make_pair(i,j));
        setContourMatrix(i,j,getContourMatrix(i2,j2));
        setContourMatrix(i2,j2,std::make_pair(-1,-1));
    }
    /* Neither end of new line is shared by a contour, so create a new contour
     * with endpoints (i,j) and (i,j+1) */
    else {
        setContourMatrix(i,j,std::make_pair(i2,j2));
        setContourMatrix(i2,j2,std::make_pair(i,j));
        numOpenLoops_++;
    }
}
