#ifndef LATTICE_H
#define LATTICE_H
#include <string>
#include "enums.h"

class Lattice {
    public:
        Lattice() { };
        ~Lattice();
        void initArrays(int m, int n);

        void setUpdated(bool updated) { updated_ = updated; };
        bool getUpdated() const { return updated_; };
        int getHeight() const { return m_; };
        int getWidth() const { return n_; };
        Number getNumber(int i, int j) const { return numbers_[i][j]; };
        void setNumber(int i, int j, Number num) { numbers_[i][j] = num; };
        Edge getHLine(int i, int j) const { return hlines_[i][j]; }
        virtual void setHLine(int i, int j, Edge edge) { hlines_[i][j] = edge; };
        Edge getVLine(int i, int j) const { return vlines_[i][j]; };
        virtual void setVLine(int i, int j, Edge edge) { vlines_[i][j] = edge; };

    protected:
        void cleanArrays();

        bool init_ = false;
        bool updated_ = true;
        int m_;     /* number of rows */
        int n_;     /* number of columns */
        Number ** numbers_;
        Edge ** hlines_;
        Edge ** vlines_;
};

#endif
