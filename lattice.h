#ifndef LATTICE_H
#define LATTICE_H
#include <string>
#include "enums.h"

class Lattice {
    public:
        Lattice();
        Lattice(int m, int n);
        ~Lattice();
        void initArrays(int m, int n);
        void setNumber(int i, int j, Number num) { numbers_[i][j] = num; };
        int getHeight() const { return m_; };
        int getWidth() const { return n_; };
        Number getNumber(int i, int j) const { return numbers_[i][j]; };
        void setHLine(int i, int j, Edge edge) { hlines_[i][j] = edge; };
        Edge getHLine(int i, int j) const { return hlines_[i][j]; }
        void setVLine(int i, int j, Edge edge) { vlines_[i][j] = edge; };
        Edge getVLine(int i, int j) const { return vlines_[i][j]; };

    private:
        bool init_ = false;
        int m_;     // number of rows
        int n_;     // number of columns
        Number ** numbers_;
        Edge ** hlines_;
        Edge ** vlines_;
};

#endif
