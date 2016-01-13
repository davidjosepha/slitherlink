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
#include <dirent.h>

int main(int argc, char * argv[]) {
    int depth = 1;
    
    DIR *pDir;
    struct dirent *pDirent;
    char * dirname = "../puzzles/";
    pDir = opendir(dirname);
    while ((pDirent = readdir(pDir)) != NULL) {
        std::stringstream ss;
        std::string s;
        ss << pDirent->d_name;
        ss >> s;
        if (s.find(".slk") != -1) {
            
            std::cout << "puzzles/" << s ;
            std::string fileName = dirname + s;
            depth = 10;

            Grid grid;
            Import importer = Import(grid, fileName);
            Export exporter = Export(grid);

            Rule rules[NUM_RULES];
            initRules(rules);
            Contradiction contradictions[NUM_CONTRADICTIONS];
            initContradictions(contradictions);
            Solver solver = Solver(grid, rules, contradictions, depth);

            if (grid.isSolved()) {
                std::cout << " solved\n";
            } else {
                std::cout << " not solved\n";
            }
        }
    }
    
    clock_t startTime, endTime;
    startTime = clock();
    
    std::ifstream inputFile;
    inputFile.open("input.txt");
    std::string fileName;
    std::getline(inputFile, fileName);
    inputFile >> depth;

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
        if (solver.testContradictions()) {
            std::cout << "Stopped solving due to a contradiction\n";
        } else {
            std::cout << "Still not solved!\n";
        }
    }

    endTime = clock();
    float diff = ((float)endTime - (float)startTime) / CLOCKS_PER_SEC;
    std::cout << "Time to solve:\t" << diff << " seconds" << std::endl;

    return 0;
}
