#ifndef CONTOUR_H
#define CONTOUR_H

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
        /* TODO: Replace with std::pain<int, int> */
        int start_[2];
        int end_[2];
};

#endif
