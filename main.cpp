#include <iostream>
#include "export.h"
#include "import.h"
#include "lattice.h"
#include "solver.h"
#include "grid.h"
#include <stdio.h>
#include <time.h>

int main(int argc, char * argv[]) {
    clock_t startTime, endTime;
    startTime = clock();

    Grid grid;
    Import importer = Import(grid);
    Export exporter = Export(grid);
    std::cout << "Before:" << std::endl;
    exporter.print();

    Solver solver = Solver(grid, 1);

    std::cout << "After:" << std::endl;
    exporter.print();

    if (grid.isSolved()) {
        std::cout << "we solved it!\n";
    } else {
        std::cout << "still not solved!\n";
    }

    endTime = clock();
    float diff = ((float)endTime - (float)startTime) / CLOCKS_PER_SEC;
    std::cout << "Time to solve:\t" << diff << " seconds" << std::endl;

    return 0;
}
