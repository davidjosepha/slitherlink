#include "contour.h"
#include "coordinates.h"

/* Initialize a contour with end points
 * (starti, startj) and (endi, endj) */
Contour::Contour(int starti, int startj, int endi, int endj) {
    start_.i = starti;
    start_.j = startj;
    end_.i = endi;
    end_.j = endj;
}

/* Checks whether the contour instance passed as
 * input shares an end point with this contour. */
bool Contour::sharesEndpoint(Contour const & contour) const {
    return ((start_.i == contour.start_.i && start_.j == contour.start_.j) ||
            (start_.i == contour.end_.i && start_.j == contour.end_.j) ||
            (end_.i == contour.start_.i && end_.j == contour.start_.j) ||
            (end_.i == contour.end_.i && end_.j == contour.end_.j));
}

/* Add another contour as part of this contour. If
 * it doesn't share any endpoints, nothing happens.
 * Sets closed_ to true if new contour closes the
 * contour. */
void Contour::addContour(Contour & contour) {
    if (start_.i == contour.start_.i && start_.j == contour.start_.j) {
        start_.i = contour.end_.i;
        start_.j = contour.end_.j;
        length_++;
    } else if (start_.i == contour.end_.i && start_.j == contour.end_.j) {
        start_.i = contour.start_.i;
        start_.j = contour.start_.j;
        length_++;
    } else if (end_.i == contour.start_.i && end_.j == contour.start_.j) {
        end_.i = contour.end_.i;
        end_.j = contour.end_.j;
        length_++;
    } else if (end_.i == contour.end_.i && end_.j == contour.end_.j) {
        end_.i = contour.start_.i;
        end_.j = contour.start_.j;
        length_++;
    }

    if (start_.i == end_.i && start_.j == start_.j) {
        closed_ = true;
    }
}
