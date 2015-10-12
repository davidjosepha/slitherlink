#ifndef CONTOUR_H
#define CONTOUR_H

class Contour {
    public:
        Contour() { };
        Contour(int starti, int startj, int endi, int endj);
        int getLength() { return length_; }
        bool sharesEndpoint(Contour & contour);
        void addContour(Contour & contour);

    private:
        int length_;
        int start_[2];
        int end_[2];
};

#endif
