#include "grid.h"
#include <vector>
#include <iostream>
#include <string>
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

void Grid::setHLine(int i, int j, Edge edge) {

    hlines_[i][j] = edge;

    if (edge == LINE) {
        Contour newContour = Contour(i, j, i, j+1);
        mergeContours(newContour);
        contours_.push_back(newContour);
    }
}

void Grid::setVLine(int i, int j, Edge edge) {

    vlines_[i][j] = edge;

    if (edge == LINE) {
        Contour newContour = Contour(i, j, i+1, j);
        mergeContours(newContour);
        contours_.push_back(newContour);
    }
}
