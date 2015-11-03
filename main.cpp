#include <iostream>
#include <fstream>
#include <stdlib.h>
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

int main(int argc, char * argv[]) {
    clock_t startTime, endTime;
    startTime = clock();
    int depth = 1;
    
    std::ifstream inputFile;
    inputFile.open("input.txt");
    std::string fileName;
    std::getline(inputFile, fileName);
    inputFile >> depth;
    

    /*if (argc == 3) {
        filename = argv[1];
        depth = std::stoi(argv[2]);
    } else if (argc == 2) {
        filename = argv[1];
    } else {
        std::cout << "Usage: ./slitherlink <slk-file-location> <depth>\n";
        return 0;
    }*/

    Grid grid;
    Import importer = Import(grid, fileName);
    Export exporter = Export(grid);
    std::cout << "Before:" << std::endl;
    exporter.print();

    Rule rules[NUM_RULES];
    initRules(rules);
    Contradiction contradictions[NUM_CONTRADICTIONS];
    initContradictions(contradictions);
    Solver solver = Solver(grid, rules, contradictions, depth);

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
