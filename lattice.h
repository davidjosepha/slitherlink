#ifndef LATTICE_H
#define LATTICE_H
#include <string>
#include "enums.h"

class Lattice {
    public:
        Lattice();
        ~Lattice();
        void initArrays(int m, int n);
        void setNumber(int i, int j, Number num);
        Number getNumber(int i, int j);
        void setHLine(int i, int j, Edge edge);
        Edge getHLine(int i, int j);
        void setVLine(int i, int j, Edge edge);
        Edge getVLine(int i, int j);

    private:
        bool init_ = false;
        int m_;     // number of rows
        int n_;     // number of columns
        Number ** numbers_;
        Edge ** hlines_;
        Edge ** vlines_;
};

#endif
