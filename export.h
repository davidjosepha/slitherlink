#ifndef EXPORT_H
#define EXPORT_H
#include <string>
#include "enums.h"
#include "lattice.h"

class Export {
    public:
        Export(Lattice & lattice);
        void print();

    private:
        char formatNumber(int i, int j) const;
        char formatHLine(int i, int j) const;
        char formatVLine(int i, int j) const;

        Lattice * lattice_;
};

#endif
