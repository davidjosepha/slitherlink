#include "contour.h"

/* Initialize a contour with end points
 * (starti, startj) and (endi, endj) */
Contour::Contour(int starti, int startj, int endi, int endj) {
    start_[0] = starti;
    start_[1] = startj;
    end_[0] = endi;
    end_[1] = endj;
}

/* Checks whether the contour instance passed as
 * input shares an end point with this contour. */
bool Contour::sharesEndpoint(Contour & contour) {
    return ((start_[0] == contour.start_[0] && start_[1] == contour.start_[1]) ||
            (start_[0] == contour.end_[0] && start_[1] == contour.end_[1]) ||
            (end_[0] == contour.start_[0] && end_[1] == contour.start_[1]) ||
            (end_[0] == contour.end_[0] && end_[1] == contour.end_[1]));
}

/* Add another contour as part of this contour. If
 * it doesn't share any endpoints, nothing happens.
 * Sets closed_ to true if new contour closes the
 * contour. */
void Contour::addContour(Contour & contour) {
    if (start_[0] == contour.start_[0] && start_[1] == contour.start_[1]) {
        start_[0] = contour.end_[0];
        start_[1] = contour.end_[1];
        length_++;
    } else if (start_[0] == contour.end_[0] && start_[1] == contour.end_[1]) {
        start_[0] = contour.start_[0];
        start_[1] = contour.start_[1];
        length_++;
    } else if (end_[0] == contour.start_[0] && end_[1] == contour.start_[1]) {
        end_[0] = contour.end_[0];
        end_[1] = contour.end_[1];
        length_++;
    } else if (end_[0] == contour.end_[0] && end_[1] == contour.end_[1]) {
        end_[0] = contour.start_[0];
        end_[1] = contour.start_[1];
        length_++;
    }

    if (start_[0] == end_[0] && start_[1] == start_[1]) {
        closed_ = true;
    }
}
