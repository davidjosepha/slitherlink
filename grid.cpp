#include "grid.h"
#include <cassert>
#include <vector>
#include "contour.h"
#include "enums.h"


/*
 * Merge a newly created contour with other contours if one of their endpoints match.
 * This function assumes that all previous contours are distinct, and will ensure that by
 * the end of the function, all contours remain distinct.
 */
void Grid::mergeContours(Contour & newContour) {
    for (int n = 0; n < contours_.size(); n++) {
        if (newContour.sharesEndpoint(contours_[n])) {
            newContour.addContour(contours_[n]);

            /* delete old contour after merging */
            contours_[n] = contours_.back();
            contours_.pop_back();

            /* attempt to merge again, if possible */
            mergeContours(newContour);
            return;
        }
    }
}
/*
 * Copies grid for the purpose of making a guess.
 */
void Grid::copy(Grid & newGrid) {
    newGrid.initArrays(getHeight(), getWidth());
    newGrid.contours_ = std::vector<Contour>(contours_);

    for (int i = 0; i < getHeight(); i++) {
        for (int j = 0; j < getWidth(); j++) {
            newGrid.setNumber(i, j, getNumber(i,j));
        }
    }

    for (int i = 0; i < getHeight()+1; i++) {
        for (int j = 0; j < getWidth(); j++) {
            newGrid.setHLine(i, j, getHLine(i,j));
        }
    }

    for (int i = 0; i < getHeight(); i++) {
        for (int j = 0; j < getWidth()+1; j++) {
            newGrid.setVLine(i, j, getVLine(i,j));
        }
    }
}

/*
 * Set a horizontal line to a given edge type, checking to see if this change overwrites
 * any previous value set to that position. If a line is added, create a new contour and
 * attempt to merge that contour to any adjacent contours.
 */
bool Grid::setHLine(int i, int j, Edge edge) {
    assert(0 <= i && i < m_+1 && 0 <= j && j < n_);

    Edge prevEdge = getHLine(i, j);
    if (prevEdge == EMPTY) {
        hlines_[i][j] = edge;
    } else if (prevEdge != edge) {
        return false;
    }

    if (edge == LINE) {
        Contour newContour = Contour(i, j, i, j+1);
        mergeContours(newContour);
        contours_.push_back(newContour);
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
    }

    if (edge == LINE) {
        Contour newContour = Contour(i, j, i+1, j);
        mergeContours(newContour);
        contours_.push_back(newContour);
    }

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

bool Grid::isSolved() const {
    if (contours_.size() != 1 || !contours_[0].isClosed()) {
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
