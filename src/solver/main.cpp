#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <time.h>
#include "contradiction.h"
#include "contradictions.h"
#include "export.h"
#include "grid.h"
#include "import.h"
#include "lattice.h"
#include "rule.h"
#include "rules.h"
#include "solver.h"

#define MAX_DEPTH 10

int main(int argc, char * argv[]) {
    clock_t startTime, endTime;
    startTime = clock();
    
    Rule rules[NUM_RULES];
    initRules(rules);
    Contradiction contradictions[NUM_CONTRADICTIONS];
    initContradictions(contradictions);

    for (int i = 1; i < argc; i++) {
        char * filename = argv[i];
        std::cout << "Puzzle: " << filename << std::endl;

        Grid grid;
        Import importer = Import(grid, filename);
        Export exporter = Export(grid);

        Solver solver = Solver(grid, rules, contradictions, MAX_DEPTH);

        exporter.print();

        if (grid.isSolved()) {
            std::cout << "Solved" << std::endl;
        } else {
            if (solver.testContradictions()) {
                std::cout << "Invalid puzzle" << std::endl;
            } else if (solver.hasMultipleSolutions()) {
                std::cout << "Puzzle has multiple solutions" << std::endl;
            } else {
                std::cout << "Not solved" << std::endl;
            }
        }
    }

    endTime = clock();
    float diff = ((float)endTime - (float)startTime) / CLOCKS_PER_SEC;
    std::cout << "Total time:\t" << diff << " seconds" << std::endl;

    return EXIT_SUCCESS;
}
