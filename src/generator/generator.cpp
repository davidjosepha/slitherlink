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
    factor_ = .50 ;

    srand(time(NULL));
    Rule rules_[NUM_RULES];
    initRules(rules_);
    Contradiction contradictions_[NUM_CONTRADICTIONS];
    initContradictions(contradictions_);

    Import importer = Import(grid_, m_, n_);
    Export exporter = Export(grid_);
    LoopGen loopgen = LoopGen(m_, n_, grid_);
    initArrays();
    
    int selectedRules[NUM_RULES - NUM_CONST_RULES];
    for (int i = 0; i < NUM_RULES - NUM_CONST_RULES; i++) {
        selectedRules[i] = i;
    }
    selectedRules_ = selectedRules;

    // TODO: maybe modify selected rules 
    
    Solver solver = Solver(grid_, rules_, contradictions_, selectedRules_, NUM_RULES - NUM_CONST_RULES, 0);

    // exporter.print();
    grid_.resetGrid();

//    // doesn't work yet :/
//    // deleteNumbers(solver);
    setCounts();
    printf("%i,%i,%i\n", oneCount_, twoCount_, threeCount_);
//    
//    int count = 0;
//    int i = rand() % (m_) + 1;
//    int j = rand() % (n_) + 1;
//    Number oldNum = grid_.getNumber(i, j);
//    while (count < ((m_)*(n_)*2/3 + 10)) {
//        count++;
//        int count2 = 0;
//        do {
//            i = rand() % (m_) + 1;
//            j = rand() % (n_) + 1;
//            oldNum = grid_.getNumber(i, j);
//            count2 ++;
//            if (count2 > n_+m_) {
//                if (eligible(i,j) or oldNum == NONE) {
//                    count+= (m_+n_)/2;
//                    printf("0:%i, 1:%i, 2:%i, 3:%i\n", zeroCount_, oneCount_, twoCount_, threeCount_);
//                    printf("can't balance: %i, %i, num: %i\n", i, j, oldNum-1);
//                    break;
//                }
//            }
//        } while (!isBalanced(i, j, oldNum));
//        eliminateNumber(i, j);
//        //exporter.print();
//
//        // TODO: maybe modify selected rules 
//        
//        solver = Solver(grid_, rules_, contradictions_, selectedRules_, NUM_RULES - NUM_CONST_RULES, 1);
//        if (!grid_.isSolved()) {
//            grid_.setNumber(i, j, oldNum);
//        } else {
//            minusCounts(oldNum);
//        }
//        grid_.resetGrid();
//    }
//
////    // TODO: maybe modify selected rules 
////    
    reduceNumbers();
    solver = Solver(grid_, rules_, contradictions_, selectedRules_, NUM_RULES - NUM_CONST_RULES, 1);
    printf("here's a new puzzle:\n");
    exporter.print();
    printf("here it is unsolved:\n");
    grid_.resetGrid();
    exporter.print();
    printf("0:%i, 1:%i, 2:%i, 3:%i\n", zeroCount_, oneCount_, twoCount_, threeCount_);
    destroyArrays();
}


void Generator::deleteNumbers(){ 
    setCounts();
    printf("%i,%i,%i\n", oneCount_, twoCount_, threeCount_);
    int count = 0;
    int i = rand() % (m_) + 1;
    int j = rand() % (n_) + 1;
    Number oldNum = grid_.getNumber(i, j);
    while (count < ((m_)*(n_)*2/3 + 10)) {
        count++;
        int count2 = 0;
        while (true) {
            i = rand() % (m_) + 1;
            j = rand() % (n_) + 1;
            oldNum = grid_.getNumber(i, j);
            if (isBalanced(i, j, oldNum)) {
                break;
            }
            count2 ++;
            if (count2 > n_+m_) {
                if (eligible(i,j) or oldNum == NONE) {
                    count+= (m_+n_)/2;
                    // printf("Counts: %i,%i,%i\n", oneCount_, twoCount_, threeCount_);
                    // printf("can't balance: %i, %i, num: %i\n", i, j, oldNum-1);
                    break;
                }
            }
        }
        eliminateNumber(i, j);
        //exporter.print();

        // TODO: maybe modify selected rules 
        
        Solver solver = Solver(grid_, rules_, contradictions_, selectedRules_, NUM_RULES - NUM_CONST_RULES, 1);
        if (!grid_.isSolved()) {
            grid_.setNumber(i, j, oldNum);
        } else {
            minusCounts(oldNum);
        }
        grid_.resetGrid();
    }
}


bool Generator::isBalanced(int i, int j, Number num){
    if (eligible(i, j)){
        if (num == ZERO) {
            return true;
        } if (num == THREE) {
            return (threeCount_*2.1+1 > 3*oneCount_ & threeCount_*5.2+1 > 3*twoCount_);
        } if (num == ONE) {
            return (oneCount_*3.2+1 > 2*threeCount_ & oneCount_*5.2+1 > 2*twoCount_);
        } if (num == TWO) {
            return (twoCount_*2.1+1 > 5*oneCount_ & twoCount_*3.1+1 > 5*threeCount_);
        }   
    }
    return false;
}

bool Generator::isBalanced(int i, int j) {
    float moa = 1.1;
    Number num = grid_.getNumber(i, j);
    if (num == THREE) {
        return (threeCount_*2*moa >= 3*oneCount_ & threeCount_*5*moa >= 3*twoCount_);
    } else if (num == TWO) {
        return (twoCount_*2.1+1 >= 5*oneCount_ & twoCount_*3*moa >= 5*threeCount_);
    } else if (num == ONE) {
        return (oneCount_*3*moa >= 2*threeCount_ & oneCount_*5*moa >= 2*twoCount_);
    } else {
        return false;
    }
}

void Generator::setCounts(){
    oneCount_ = 0;
    twoCount_ = 0;
    threeCount_ = 0;
    for (int i = 1; i <= m_; i++) {
        for (int j = 1; j <= n_; j++) {
            Number oldNum = grid_.getNumber(i, j);
            if (oldNum == ZERO) {
                zeroCount_ ++;
            }else if (oldNum == ONE) {
                oneCount_ ++;
            } else if (oldNum == TWO) {
                twoCount_ ++;
            } else if (oldNum == THREE) {
                threeCount_ ++;
            } 
        }
    }
}

void Generator::minusCounts(Number num){
    if (num == ZERO) {
        zeroCount_ --;
    }else if (num == ONE) {
        oneCount_ --;
    } else if (num == TWO) {
        twoCount_ --;
    } else if (num == THREE) {
        threeCount_ --;
    }
}

void Generator::plusCounts(Number num){
    if (num == ZERO) {
        zeroCount_ ++;
    }else if (num == ONE) {
        oneCount_ ++;
    } else if (num == TWO) {
        twoCount_ ++;
    } else if (num == THREE) {
        threeCount_ ++;
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
    
    
    while (numberCount_ > (m_*n_)*factor_) {
        std::cout << "Number count:\t" << numberCount_ << std::endl;
        
        findNumberToRemove();
        
        
        eligibleCoordinates_.clear();
        grid_.resetGrid();
        Export exporter = Export(grid_);
        exporter.print();
        printf("0:%i, 1:%i, 2:%i, 3:%i\n", zeroCount_, oneCount_, twoCount_, threeCount_);
    }
}

/* Finds a number to remove from the grid while keeping exactly one solution */
void Generator::findNumberToRemove() {   
    fillSingleEligible();
    bool coordsFound = false;
    
    while (!eligibleCoordinates_.empty() && !coordsFound) {
        int random = rand() % eligibleCoordinates_.size();
        Coordinates attempt = eligibleCoordinates_.at(random);
        eligibleCoordinates_.erase(eligibleCoordinates_.begin() + random);
        
        if (isBalanced(attempt.i, attempt.j)) {
            removeNumber(attempt.i, attempt.j);
            if (!checkIfSolved()) {
                setOldNumber(attempt.i, attempt.j);
                grid_.resetGrid();
                markNecessary(attempt.i, attempt.j);
            
            } else {
                ineligibleCoordinates_.push_back(attempt);
                coordsFound = true;
                numberCount_ --;
                minusCounts(oldNumbers_[attempt.i-1][attempt.j-1]);
            }
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
void Generator::fillSingleEligible() {
    for (int i = 1; i < m_+1; i++) {
        for (int j = 1; j < n_+1; j++) {
            if (eligible(i, j)) {
                Coordinates coords = { i, j };
                eligibleCoordinates_.push_back(coords);
            }
        }
    }
}


void Generator::fillEligibleVectors() {
    for (int i = 1; i < m_+1; i++) {
        for (int j = 1; j < n_+1; j++) {
            if (eligible(i, j)) {
                Coordinates coords  = { i, j };
                
                if (grid_.getNumber(i,j) == ZERO) {
                    eligibleZeroCoordinates_.push_back(coords);
                } else if (grid_.getNumber(i,j) == ONE) {
                    eligibleOneCoordinates_.push_back(coords);;
                } else if (grid_.getNumber(i,j) == TWO) {
                    eligibleTwoCoordinates_.push_back(coords);
                } else if (grid_.getNumber(i,j) == THREE) {
                    eligibleThreeCoordinates_.push_back(coords);
                }
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

    // TODO: maybe modify selected rules 
    
    Solver solver = Solver(grid_, rules_, contradictions_, selectedRules_, NUM_RULES - NUM_CONST_RULES, 1);
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
            plusCounts(grid_.getNumber(popped.i, popped.j));
            found = true;
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