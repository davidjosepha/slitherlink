#ifndef CONTOUR_H
#define CONTOUR_H

#include "structs.h"

class Contour {
    public:
        Contour() { };
        Contour(int starti, int startj, int endi, int endj);
        int getLength() const { return length_; };
        bool sharesEndpoint(Contour const & contour) const;
        void addContour(Contour & contour);
        bool isClosed() const { return closed_; };

    private:
        int length_ = 0;
        bool closed_ = false;
        Coordinates start_;
        Coordinates end_;
};

#endif
