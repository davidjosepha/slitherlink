#include "grid.h"
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
            contours_[n] = contours_.back(); // Delete the contour at n since we've added it to the new one
            contours_.pop_back();
            mergeContours(newContour); // There might be another contour to merge so run method again
            break;
        }
    }
}

bool Grid::setHLine(int i, int j, Edge edge) {

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

bool Grid::setVLine(int i, int j, Edge edge) {

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

bool Grid::numberSatisfied(int i, int j) {

    Number number = numbers_[i][j];
    if (number == NONE) {
        return true;
    }

    int numLines = 0;
    if (vlines_[i][j] == LINE) {
        numLines++;
    }
    if (vlines_[i][j+1] == LINE) {
        numLines++;
    }
    if (hlines_[i][j] == LINE) {
        numLines++;
    }
    if (hlines_[i+1][j] == LINE) {
        numLines++;
    }

    return numLines+1 == number;
}

bool Grid::isSolved() {
    if (contours_.size() != 1 or (contours_.size() > 0 and not contours_[0].isClosed())) {
        return false;
    }
    for (int i = 0; i < m_; i++) {
        for (int j = 0; j < n_; j++) {
            if (not numberSatisfied(i,j)) {
                return false;
            }
        }
    }
    return true;
}
