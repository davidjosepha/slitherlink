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
#include <iostream>
#include <stack>

/* Generator constructor */
Generator::Generator(int m, int n) {
    m_ = m;
    n_ = n;
    
    numberCount_ = m_*n_;
    // 1count_, 2count_, 3count_ = 0;
    factor_ = 2;

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
    
    // reduceNumbers();
    solver = Solver(grid_, rules_, contradictions_, 1);
    printf("here's a new puzzle:\n");
    exporter.print();
    printf("here it is unsolved:\n");
    grid_.resetGrid();
    exporter.print();
    printf("%i,%i,%i\n", oneCount_, twoCount_, threeCount_);
    destroyArrays();
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

/* Generate a puzzle using other helper methods */
void Generator::genPuzzle() { }


void Generator::reduceNumbers() {
    
    
    while (numberCount_ > (m_*n_)/2) {
        std::cout << "Number count:\t" << numberCount_ << std::endl;
        
        findNumberToRemove();
        eligibleCoordinates_.clear();
        grid_.resetGrid();
        Export exporter = Export(grid_);
        exporter.print();
    }
}

/* Finds a number to remove from the grid while keeping exactly one solution */
void Generator::findNumberToRemove() {   
    fillEligibleVector();
    bool coordsFound = false;
    
    while (!eligibleCoordinates_.empty() && !coordsFound) {
        int random = rand() % eligibleCoordinates_.size();
        Coordinates attempt = eligibleCoordinates_.at(random);
        eligibleCoordinates_.erase(eligibleCoordinates_.begin() + random);
        //std::cout << "Eligible Size:\t" << eligibleCoordinates_.size()<< std::endl;
        removeNumber(attempt.i, attempt.j);
        if (!checkIfSolved()) {
            setOldNumber(attempt.i, attempt.j);
            grid_.resetGrid();
            markNecessary(attempt.i, attempt.j);
            
        } else {
            ineligibleCoordinates_.push_back(attempt);
            coordsFound = true;
            numberCount_ --; 
        }
    }
    
    
    if (!coordsFound && numberCount_ < m_ * n_) {
        getNecessaryCoordinate();
        numberCount_ ++;
    } else if (!coordsFound) {
        printf("yikes:\n");
    } 
}

/* Adds Coordinates of Numbers that are eligible for elimination to a vector */
void Generator::fillEligibleVector() {
    for (int i = 1; i < m_+1; i++) {
        for (int j = 1; j < n_+1; j++) {
            if (eligible(i, j)) {
                Coordinates coords = { i, j };
                eligibleCoordinates_.push_back(coords);
            }
        }
    }
}

bool Generator::checkIfSolved() {
    Rule rules_[NUM_RULES];
    initRules(rules_);
    Contradiction contradictions_[NUM_CONTRADICTIONS];
    initContradictions(contradictions_);
    grid_.resetGrid();
    Solver solver = Solver(grid_, rules_, contradictions_, 1);
    if (grid_.isSolved()) {
        return true;
    } else {
        return false;
    }
    grid_.resetGrid();
}


/* Pops Coordinates out of ineligible vector, marking each as eligible
 * until one is found that has been removed */
void Generator::getNecessaryCoordinate() {
    bool found = false;
    
    
    while (!found) {
        Coordinates popped = ineligibleCoordinates_.back();
        if (grid_.getNumber(popped.i, popped.j) == NONE) {
            markNecessary(popped.i, popped.j);
            setOldNumber(popped.i, popped.j);
            ineligibleCoordinates_.push_back(popped);
        } else {
            ineligibleCoordinates_.pop_back();
            markEligible(popped.i, popped.j);
        }
    }
}

void Generator::setOldNumber(int i, int j) {
    grid_.setNumber(i, j, oldNumbers_[i-1][j-1]);
}

/* Elimates a number at a set of coordinates */
void Generator::removeNumber(int i, int j) {
    grid_.setNumber(i, j, NONE); 
    grid_.resetGrid();
    Coordinates removed{ i, j };
    ineligibleCoordinates_.push_back(removed);
    
}

/* Elimates a number at a set of coordinates */
void Generator::eliminateNumber(int i, int j) {
    grid_.setNumber(i, j, NONE); 
    grid_.resetGrid();
    canEliminate_[i-1][j-1] = false;
}

/* Determines if a Number at Coordinates is eligible for elimination */
bool Generator::eligible(int i, int j) {
    if (canEliminate_[i-1][j-1] && (grid_.getNumber(i, j) != NONE)) {
        return true;
    } else {
        return false;
    }
}
                            
/* Marks a Number at specific Coordinates as eligible for elimination */
void Generator::markEligible(int i, int j) {
    canEliminate_[i-1][j-1] = true;
}

void Generator::markNecessary(int i, int j) {
    canEliminate_[i-1][j-1] = false;
}