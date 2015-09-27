#include "import.h"
#include <iostream>
#include <string>
#include "enums.h"
#include "lattice.h"

#define POINT '.'
#define HLINE '-'
#define VLINE '|'
#define EX 'x'
#define BLANK ' '

Import::Import() { }

Import::Import(Lattice & lattice) {
    lattice_ = lattice;
    buildLattice();
}

void Import::buildLattice() {
    std::string buffer;

    // source info
    std::getline(std::cin, buffer);

    // get dimensions
    int m, n;
    std::cin >> m;
    std::cin >> n;
    lattice_.initArrays(m, n);

    // blank lines
    std::getline(std::cin, buffer);
    std::getline(std::cin, buffer);

    // numbers
    for (int i = 0; i < m; i++) {
        std::getline(std::cin, buffer);
        importNumberRow(i, buffer);
    }

    // blank line
    std::getline(std::cin, buffer);

    // horizontal lines
    for (int i = 0; i < m+1; i++) {
        std::getline(std::cin, buffer);
        importHLineRow(i, buffer);
    }

    // blank line
    std::getline(std::cin, buffer);

    // vertical lines
    for (int i = 0; i < m; i++) {
        std::getline(std::cin, buffer);
        importVLineRow(i, buffer);
    }
}

void Import::importNumberRow(int i, std::string row) {
    for (std::string::size_type j = 0; j < row.size(); j++) {
        char c = row[j];
        switch (c) {
            case '0':
                lattice_.setNumber(i, j, ZERO);
                break;
            case '1':
                lattice_.setNumber(i, j, ONE);
                break;
            case '2':
                lattice_.setNumber(i, j, TWO);
                break;
            case '3':
                lattice_.setNumber(i, j, THREE);
                break;
            default:
                lattice_.setNumber(i, j, NONE);
                break;
        }
    }
}

void Import::importHLineRow(int i, std::string row) {
    for (std::string::size_type j = 0; j < row.size(); j++) {
        char c = row[j];
        switch (c) {
            case '-':
                lattice_.setHLine(i, j, LINE);
                break;
            case 'x':
                lattice_.setHLine(i, j, NLINE);
                break;
            default:
                lattice_.setHLine(i, j, EMPTY);
                break;
        }
    }
}

void Import::importVLineRow(int i, std::string row) {
    for (std::string::size_type j = 0; j < row.size(); j++) {
        char c = row[j];
        switch (c) {
            case '-':
                lattice_.setVLine(i, j, LINE);
                break;
            case 'x':
                lattice_.setVLine(i, j, NLINE);
                break;
            default:
                lattice_.setVLine(i, j, EMPTY);
                break;
        }
    }
}
