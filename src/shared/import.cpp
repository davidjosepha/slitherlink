#include "import.h"
#include <iostream>
#include <fstream>
#include <string>
#include "enums.h"
#include "grid.h"

#define POINT '.'
#define HLINE '-'
#define VLINE '|'
#define EX 'x'
#define BLANK ' '

/* Empty constructor */
Import::Import() { }




/* Constructor taking as input a lattice to be imported into */
Import::Import(Grid & lattice, std::string filename) {
    lattice_ = &lattice;
    buildLattice(filename);
}


/* Constructor for empty lattice */
Import::Import(Grid & lattice, int m, int n) {
    lattice_ = &lattice;
    buildEmptyLattice(m, n);
}


/* Reads from stdin and initializes a lattice based on given
 * dimensions and three separate grids, one each for numbers,
 * horizontal lines, and vertical lines. */
void Import::buildLattice(std::string filename) {
    std::string buffer;
    std::ifstream slkfile(filename);

    if (slkfile.is_open()) {

        /* source info */
        std::getline(slkfile, buffer);

        /* get dimensions */
        int m, n;
        slkfile >> m;
        slkfile >> n;
        lattice_->initArrays(m+2, n+2);
        lattice_->initUpdateMatrix();

        /* blank lines */
        std::getline(slkfile, buffer);
        std::getline(slkfile, buffer);

        /* numbers */
        for (int i = 0; i < m; i++) {
            std::getline(slkfile, buffer);
            importNumberRow(i+1, buffer);
        }

        /* blank line */
        std::getline(slkfile, buffer);

        /* horizontal lines */
        for (int i = 0; i < m+1; i++) {
            std::getline(slkfile, buffer);
            importHLineRow(i+1, buffer);
        }
        for (int j = 0; j < n+2; j++) {
            lattice_->setHLine(0, j, NLINE);
            lattice_->setHLine(m+2, j, NLINE);
        }

        /* blank line */
        std::getline(slkfile, buffer);

        /* vertical lines */
        for (int j = 0; j < n+3; j++) {
            lattice_->setVLine(0, j, NLINE);
            lattice_->setVLine(m+1, j, NLINE);
        }
        for (int i = 0; i < m+2; i++) {
            lattice_->setVLine(i, 0, NLINE);
            lattice_->setVLine(i, n+2, NLINE);
        }

        for (int i = 0; i < m; i++) {
            std::getline(slkfile, buffer);
            importVLineRow(i+1, buffer);
        }
        slkfile.close();
    } else {
        std::cout << "Unable to open file\n";
    }
}

/* Initializes an empty lattice based on given dimensions */
void Import::buildEmptyLattice(int m, int n) {

    lattice_->initArrays(m+2, n+2);
    lattice_->initUpdateMatrix();

    for (int i = 0; i < m+1; i++) {
        lattice_->setHLine(i+1, 0, NLINE);
    }

    for (int i = 0; i < m+2; i++) {
        lattice_->setVLine(i, 0, NLINE);
        lattice_->setVLine(i, n+2, NLINE);
    }

    for (int j = 0; j < n+3; j++) {
        lattice_->setVLine(0, j, NLINE);
        lattice_->setVLine(m+1, j, NLINE);
    }

    for (int j = 0; j < n+2; j++) {
        lattice_->setHLine(0, j, NLINE);
        lattice_->setHLine(m+2, j, NLINE);
    }

    for (int j = 0; j < m+3; j++) {
        lattice_->setHLine(j, 0, NLINE);
        lattice_->setHLine(j, n+1, NLINE);
    }

}

/* Helper function for reading a line from stdin and
 * interpreting 0-3 as their corresponding values in
 * the Number enumeration. */
void Import::importNumberRow(int i, std::string row) {
    for (std::string::size_type j = 0; j < row.size(); j++) {
        char c = row[j];
        switch (c) {
            case '0':
                lattice_->setNumber(i, j+1, ZERO);
                break;
            case '1':
                lattice_->setNumber(i, j+1, ONE);
                break;
            case '2':
                lattice_->setNumber(i, j+1, TWO);
                break;
            case '3':
                lattice_->setNumber(i, j+1, THREE);
                break;
            default:
                lattice_->setNumber(i, j+1, NONE);
                break;
        }
    }
}


/* Helper function for reading a line from stdin and
 * interpreting '-' and 'x' as their corresponding values in
 * the Number enumeration. */
void Import::importHLineRow(int i, std::string row) {
  lattice_->setHLine(i, 0, NLINE);
    for (std::string::size_type j = 0; j < row.size(); j++) {

        char c = row[j];
        switch (c) {
            case '-':
                lattice_->setHLine(i, j+1, LINE);
                break;
            case 'x':
                lattice_->setHLine(i, j+1, NLINE);
                break;
            default:
                lattice_->setHLine(i, j+1, EMPTY);
                break;
        }
    }
    lattice_->setHLine(i, row.size()+1, NLINE);
}

/* Helper function for reading a line from stdin and
 * interpreting '-' and 'x' as their corresponding values in
 * the Number enumeration. */
void Import::importVLineRow(int i, std::string row) {

    for (std::string::size_type j = 0; j < row.size(); j++) {
        char c = row[j];
        switch (c) {
            case '-':
                lattice_->setVLine(i, j+1, LINE);
                break;
            case 'x':
                lattice_->setVLine(i, j+1, NLINE);
                break;
            default:
                lattice_->setVLine(i, j+1, EMPTY);
                break;
        }
    }
}
