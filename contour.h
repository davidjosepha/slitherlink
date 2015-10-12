#ifndef CONTOUR_H
#define CONTOUR_H

class Contour {
    public:
        Contour() { };
        Contour(int starti, int startj, int endi, int endj);
        int getLength() { return length_; }
        bool sharesEndpoint(Contour & contour);
        void addContour(Contour & contour);
        bool isClosed() { return closed_; }

    private:
        int length_ = 0;
        bool closed_ = false;
        int start_[2];
        int end_[2];
};

#endif
