#include "generator.h"
#include <stdlib.h>
#include <time.h>
#include "loopgen.h"
#include "../solver/contradiction.h"
#include "../solver/contradictions.h"
#include "../solver/rule.h"
#include "../solver/rules.h"
#include "../solver/solver.h"
#include "../shared/structs.h"
#include "../shared/export.h"
#include "../shared/import.h"
#include <stack>

/* Generator constructor */
Generator::Generator(int m, int n) {
    m_ = m;
    n_ = n;
    
    numberCount_ = m_*n_;
    // 1count_, 2count_, 3count_ = 0;
    srand(time(NULL));
    Rule rules_[NUM_RULES];
    initRules(rules_);
    Contradiction contradictions_[NUM_CONTRADICTIONS];
    initContradictions(contradictions_);

    Import importer = Import(grid_, m_, n_);
    Export exporter = Export(grid_);
    LoopGen loopgen = LoopGen(m_, n_, grid_);
    initArrays();

    Solver solver = Solver(grid_, rules_, contradictions_, 0);
    exporter.print();
    grid_.resetGrid();
    setCounts();
    printf("%i,%i,%i\n", oneCount_, twoCount_, threeCount_);

    /* TODO: Clean this up */
    /* please */
    int count = 0;
    int i = rand() % (m_) + 1;
    int j = rand() % (n_) + 1;
    Number oldNum = grid_.getNumber(i, j);
    while (count < ((m_)*(n_)*2/3)) {
        count++;
        int count2 = 0;
        do {
            i = rand() % (m_) + 1;
            j = rand() % (n_) + 1;
            oldNum = grid_.getNumber(i, j);
            count2 ++;
            if (count2 > n_*m_/3) 
                if (eligible(i,j)) {
                    printf("failure\n");
                    count+= 10;
                    break;
                }
        } while (!isBalanced(i, j, oldNum));

        eliminateNumber(i, j);
        //exporter.print();
        solver = Solver(grid_, rules_, contradictions_, 1);
        if (!grid_.isSolved()) {
            grid_.setNumber(i, j, oldNum);
        } else {
            changeCounts(oldNum);
        }
        grid_.resetGrid();
    }
    solver = Solver(grid_, rules_, contradictions_, 1);
    printf("here's new puzzle:\n");
    exporter.print();
    printf("here is it unsolved:\n");
    grid_.resetGrid();
    exporter.print();
    printf("%i,%i,%i\n", oneCount_, twoCount_, threeCount_);
    destroyArrays();
}

/* Generate a puzzle using other helper methods */
void Generator::genPuzzle() { }

/* Remove numbers from the grid while keeping exactly one solution */
void Generator::reduceNumbers() {
    
    
}


/* Elimates a number at a set of coordinates */
void Generator::eliminateNumber(int i, int j) {
    grid_.setNumber(i, j, NONE); 
    grid_.resetGrid();
    canEliminate_[i-1][j-1] = false;
}

/* Determines if a set of coordinates are eligible for elimination */
bool Generator::eligible(int i, int j) {
    return canEliminate_[i-1][j-1];
}

bool Generator::isBalanced(int i, int j, Number num){
    if (eligible(i, j)){
        if (num == ZERO) {
            return true;
        }
        int onePlusThree = oneCount_ + threeCount_;
        if (onePlusThree*1.2-1 > twoCount_) {
            if (oneCount_*1.2-1 > threeCount_ & num == ONE) {
                return true;
            } if (threeCount_*1.2-1 > oneCount_ & num == THREE) {
                return true;
            }
        } if (onePlusThree < twoCount_*1.2-1 & num == TWO) {
            return true;
        }
        
    }
    return false;
}

void Generator::setCounts(){
    for (int i = 1; i <= m_; i++) {
        for (int j = 1; j <= n_; j++) {
            Number oldNum = grid_.getNumber(i, j);
            if (oldNum == ONE) {
                oneCount_ ++;
            } else if (oldNum == TWO) {
                twoCount_ ++;
            } else if (oldNum == THREE) {
                threeCount_ ++;
            }
        }
    }
}

void Generator::changeCounts(Number num){
    if (num == ONE) {
        oneCount_ --;
    } else if (num == TWO) {
        twoCount_ --;
    } else if (num == THREE) {
        threeCount_ --;
    }
}

/* allocate memory for creating loop */
void Generator::initArrays() {
    canEliminate_ = new bool*[m_];
    oldNumbers_ = new Number*[m_];
    for (int i = 0; i < m_; i++) {
        canEliminate_[i] = new bool[n_];
        oldNumbers_[i] = new Number[n_];

        for (int j = 0; j < n_; j++) {
            canEliminate_[i][j] = true;
            oldNumbers_[i][j] = grid_.getNumber(i+1, j+1);
        }
    }
}

/* reallocate memory */
void Generator::destroyArrays() {
    for (int i = 0; i < m_; i++) {
        delete [] canEliminate_[i];
        delete [] oldNumbers_[i];
    }
    delete [] canEliminate_;
    delete [] oldNumbers_;
}