#include <iostream>
#include <time.h>
#include "export.h"
#include "grid.h"
#include "import.h"
#include "lattice.h"
#include "rules.h"
#include "solver.h"

int main(int argc, char * argv[]) {
    clock_t startTime, endTime;
    startTime = clock();

    Grid grid;
    Import importer = Import(grid);
    Export exporter = Export(grid);
    std::cout << "Before:" << std::endl;
    exporter.print();

    Rule rules[NUM_RULES];
    initRules(rules);
    Solver solver = Solver(grid, rules, 2);

    std::cout << "After:" << std::endl;
    exporter.print();

    if (grid.isSolved()) {
        std::cout << "We solved it!\n";
    } else {
        std::cout << "Still not solved!\n";
    }

    endTime = clock();
    float diff = ((float)endTime - (float)startTime) / CLOCKS_PER_SEC;
    std::cout << "Time to solve:\t" << diff << " seconds" << std::endl;

    return 0;
}
