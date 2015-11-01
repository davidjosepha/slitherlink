#ifndef LATTICE_H
#define LATTICE_H
#include <string>
#include "enums.h"

class Lattice {
    public:
        Lattice() { };
        ~Lattice();
        void initArrays(int m, int n);

        bool getUpdated() const { return updated_; };
        void setUpdated(bool updated) { updated_ = updated; };
        int getHeight() const { return m_; };
        int getWidth() const { return n_; };
        Number getNumber(int i, int j) const;
        Edge getHLine(int i, int j) const;
        Edge getVLine(int i, int j) const;
        void setNumber(int i, int j, Number num);
        virtual bool setHLine(int i, int j, Edge edge);
        virtual bool setVLine(int i, int j, Edge edge);

    protected:
        void destroyArrays();
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
