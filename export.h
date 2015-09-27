#ifndef LATTICE_H
#define LATTICE_H
#include <string>
#include "enums.h"

class Export {
    public:
        Export();
        ~Export();
        void print();

    private:
        char formatNumber(int i, int j) const;
        char formatHLine(int i, int j) const;
        char formatVLine(int i, int j) const;
};

#endif
