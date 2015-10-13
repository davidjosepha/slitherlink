#ifndef CONTRADICTION_H
#define CONTRADICTION_H
#include "enums.h"
#include "lattice.h"

class Contradiction {
    public:
        Contradiction() { };
        Contradiction(Lattice const & cont);
        int getHeight() const { return m_; };
        int getWidth() const { return n_; };
        Lattice const * getCont() const { return cont_; };

        int getNumberHeight(Orientation orient) const;
        int getNumberWidth(Orientation orient) const;
        int getHLineHeight(Orientation orient) const;
        int getHLineWidth(Orientation orient) const;
        int getVLineHeight(Orientation orient) const;
        int getVLineWidth(Orientation orient) const;

        Number getNumber(int i, int j, Orientation orient) const;
        Edge getHLine(int i, int j, Orientation orient) const;
        Edge getVLine(int i, int j, Orientation orient) const;

    private:
        int m_;
        int n_;
        Lattice const * cont_;
};

#endif
