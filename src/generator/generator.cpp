#include "generator.h"
#include <time.h>
#include <iostream>
#include <stack>
#include "loopgen.h"
#include "../shared/export.h"
#include "../shared/import.h"
#include "../shared/structs.h"
#include "../solver/contradiction.h"
#include "../solver/contradictions.h"
#include "../solver/rule.h"
#include "../solver/rules.h"
#include "../solver/solver.h"


/* Generator constructor */
Generator::Generator(int m, int n, Difficulty difficulty) {
    m_ = m;
    n_ = n;
    
    numberCount_ = m_*n_;

    buffer_ = 0;

    srand(time(NULL));
    setDifficulty(difficulty);
    createPuzzle();
    displayFinalPuzzle();
    destroyArrays();
}

/* Sets the difficulty of the puzzle by imposing limitiations on the solver's capabilities */
void Generator::setDifficulty(Difficulty difficulty) {
    setRules(difficulty);
    if (difficulty == EASY) {
        factor_ = .52;
        guessDepth_ = 2;
    } else if (difficulty == HARD) {
        factor_ = .42;
        guessDepth_ = 1;
    }
}

/* Sets which rules the solver can apply */
void Generator::setRules(Difficulty difficulty) {
    if (difficulty == EASY) {
        numberOfRules_ = 12;
    } else {
        numberOfRules_ = NUM_RULES - NUM_CONST_RULES;
    }
    selectedRules_ = new int[numberOfRules_];
    
    for (int i = 0; i < numberOfRules_; i++) {
        selectedRules_[i] = i;
    }
}

/* Creates the puzzle by importing a puzzle, 
 * creating a loop, and removing numbers */
void Generator::createPuzzle() {
    smallestCount_ = numberCount_;
    bufferReachCount_ = 0;
    Import importer = Import(grid_, m_, n_);
    LoopGen loopgen = LoopGen(m_, n_, grid_);
    
    initArrays();
    setCounts();
    grid_.copy(smallestCountGrid_);
    reduceNumbers();
}

/* Prints the puzzle in its final state, both solved and unsolved */
void Generator::displayFinalPuzzle() {
    checkIfSolved();
    displayPuzzle();
    std::cout << std::endl;
    grid_.resetGrid();
    displayPuzzle();
    
}

/* Displays the puzzle, the total count of numbers, and a count of each type */
void Generator::displayPuzzle() {
    
    Export exporter = Export(grid_);
    exporter.print();
}

/* Sets the counts of each number to the amount 
 * contained before removal of numbers */
void Generator::setCounts(){
    zeroCount_ = 0;
    oneCount_ = 0;
    twoCount_ = 0;
    threeCount_ = 0;
    for (int i = 1; i <= m_; i++) {
        for (int j = 1; j <= n_; j++) {
            Number oldNum = grid_.getNumber(i, j);
            plusCounts(oldNum);
        } 
    }
}


/* Adds to a number's count */
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

/* Subtracts from a number's count */
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
    delete [] selectedRules_;
}

/* Reduces numbers from the puzzle until a satisfactory number has been reached */
void Generator::reduceNumbers() {
    
    // Remove numbers until this count has been reached
    while (numberCount_ > ((m_*n_)*factor_ + 3)) {
        
        /* Reset the smallest count and buffer incase the required amount
        of numbers cannot be removed. */
        if (smallestCount_ > numberCount_) {
            smallestCount_ = numberCount_;
            grid_.clearAndCopy(smallestCountGrid_);
            buffer_ = (numberCount_ + (m_*n_))/2 - 2;
        }
        
        if (numberCount_ == buffer_) {
            bufferReachCount_ ++;
        }
        
        /* If the count has past the buffer three times,
         * return the grid with the smallest count of
         * of numbers that is currently known. */
        if (bufferReachCount_ == 3) {
            smallestCountGrid_.clearAndCopy(grid_);
            break;
        } 
        
        findNumberToRemove();
        eligibleCoordinates_.clear();
        
        grid_.resetGrid();
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
        
        // Checks if the number in question is needed to retain a balance
        if (isBalanced(attempt.i, attempt.j)) {
            removeNumber(attempt.i, attempt.j);
            
            // If unsolvable, bring number back and look for another
            if (!checkIfSolved()) {
                setOldNumber(attempt.i, attempt.j);
                markNecessary(attempt.i, attempt.j);
            } else {
                ineligibleCoordinates_.push_back(attempt);
                coordsFound = true;
                numberCount_ --;
                minusCounts(oldNumbers_[attempt.i-1][attempt.j-1]);
            }
        }
    }
    
    // If no more candidates, bring back the previously removed number
    if (!coordsFound && numberCount_ < m_ * n_) {
        getNecessaryCoordinate();
        numberCount_ ++;
    } 
}

/* Determines if the puzzle contains a proper ratio of Number types */
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
    
    Solver solver = Solver(grid_, rules_, contradictions_, selectedRules_, numberOfRules_, guessDepth_);
    if (grid_.isSolved()) {
        return true;
    } else {
        return false;
    }
}

/* Pops Coordinates out of ineligible vector, marking 
 * each as eligible until one is found that has been removed. 
 * This one is then marked as necessary */
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

/* Sets a space in the grid back to its original number */
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

/* Marks a Number at specific Coordinates as ineligible for elimination
 * due to its necessity to complete the puzzle at this configuration */
void Generator::markNecessary(int i, int j) {
    canEliminate_[i-1][j-1] = false;
}


/* Another method for removing numbers */
void Generator::deleteNumbers(){ 
    setCounts();
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
