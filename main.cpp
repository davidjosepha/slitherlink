#include <iostream>
#include "export.h"
#include "import.h"
#include "lattice.h"
#include "solver.h"
#include "grid.h"
#include <stdio.h>

int main(int argc, char * argv[]) {
    Grid grid;
    Import importer = Import(grid);
    Export exporter = Export(grid);
    std::cout << "Before:" << std::endl;
    exporter.print();

    Solver solver = Solver(grid);
    solver.solve();

    std::cout << "After:" << std::endl;
    exporter.print();

    if (grid.isSolved()) {
        std::cout << "we solved it!\n";
    } else {
        std::cout << "still not solved!\n";
    }
    return 0;
}
