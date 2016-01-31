#include "generator.h"
#include "../solver/import.h"
#include "../solver/export.h"
#include "../solver/contradiction.h"
#include "../solver/contradictions.h"
#include "../solver/rule.h"
#include "../solver/rules.h"
#include "../solver/solver.h"
#include <stdlib.h>
#include <time.h>

/* Generator constructor */
Generator::Generator(int m, int n) {
    m_ = m;
    n_ = n;
    srand(time(NULL));
    Rule rules_[NUM_RULES];
    initRules(rules_);
    Contradiction contradictions_[NUM_CONTRADICTIONS];
    initContradictions(contradictions_);
    Import importer = Import(grid_, "test.slk");
    Export exporter = Export(grid_);
    fillGrid();
    Solver solver = Solver(grid_, rules_, contradictions_, 0);
    grid_.resetGrid();
    int count = 0;
    while (count < ((m_-2)*(n_-2)*2)) {
        count++;
        int y = rand() % (n_-2) + 1;
        int x = rand() % (m_-2) + 1;
        Number oldNum = grid_.getNumber(y, x);
        grid_.setNumber(y, x, NONE);
        grid_.resetGrid();
        exporter.print();
        solver = Solver(grid_, rules_, contradictions_, 1);
        if (!grid_.isSolved()) {
            grid_.setNumber(y, x, oldNum);
            grid_.resetGrid();
            solver = Solver(grid_, rules_, contradictions_, 1);
        }
    }
    solver = Solver(grid_, rules_, contradictions_, 0);
    printf("here's new puzzle:\n");
    exporter.print();
    printf("here is it unsolved:\n");
    grid_.resetGrid();
    exporter.print();
}

/* Generate a puzzle using other helper methods */
void Generator::genPuzzle() { }

/* Remove numbers from the grid while keeping exactly one solution */
void Generator::reduceNumbers() { }

/* Fills grid with the appropriate numbers for a given loop */
void Generator::fillGrid() {
    for (int i = 1; i < m_-1; i++) {
        for (int j = 1; j < n_-1; j++) {
            int borderCount = int(grid_.getHLine(i,j) == LINE) +
                          int(grid_.getHLine(i+1,j) == LINE) +
                          int(grid_.getVLine(i,j) == LINE) +
                          int(grid_.getVLine(i,j+1) == LINE);
            if (borderCount == 0) {
                grid_.setNumber(i, j, ZERO);
            } else if (borderCount == 1) {
                grid_.setNumber(i, j, ONE);
            } else if (borderCount == 2) {
                grid_.setNumber(i, j, TWO);
            } else if (borderCount == 3) {
                grid_.setNumber(i, j, THREE);
            }
        }
    }
}
