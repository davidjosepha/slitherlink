#include "generator.h"
#include <stdlib.h>
#include <time.h>
#include "loopgen.h"
#include "../solver/contradiction.h"
#include "../solver/contradictions.h"
#include "../solver/rule.h"
#include "../solver/rules.h"
#include "../solver/solver.h"
#include "../shared/export.h"
#include "../shared/import.h"

/* Generator constructor */
Generator::Generator(int m, int n) {
    m_ = m;
    n_ = n;

    srand(time(NULL));
    Rule rules_[NUM_RULES];
    initRules(rules_);
    Contradiction contradictions_[NUM_CONTRADICTIONS];
    initContradictions(contradictions_);

    Import importer = Import(grid_, m_, n_);
    Export exporter = Export(grid_);
    LoopGen loopgen = LoopGen(m_, n_, grid_);
    exporter.print();

    Solver solver = Solver(grid_, rules_, contradictions_, 0);
    grid_.resetGrid();

    /* TODO: Clean this shit up */
    /* please */
    int count = 0;
    while (count < ((m_)*(n_)*2)) {
        count++;
        int i = rand() % (n_) + 1;
        int j = rand() % (m_) + 1;
        Number oldNum = grid_.getNumber(i, j);
        grid_.setNumber(i, j, NONE);
        grid_.resetGrid();
        exporter.print();
        solver = Solver(grid_, rules_, contradictions_, 1);
        if (!grid_.isSolved()) {
            grid_.setNumber(i, j, oldNum);
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

/* allocate memory for creating loop */
void Generator::initArray() {
    canEliminate_ = new bool*[m_];
    for (int i = 0; i < m_; i++) {
        canEliminate_[i] = new bool[n_];

        for (int j = 0; j < n_; j++) {
            canEliminate_[i][j] = true;
        }
    }
}

/* reallocate memory */
void Generator::destroyArray() {
    for (int i = 0; i < m_; i++) {
        delete [] canEliminate_[i];
    }
    delete [] canEliminate_;
}