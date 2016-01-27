#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h> 
#include <stdio.h>

#include <array>
#include <cassert>
#include <vector>
#include <ctime>

#include "../solver/constants.h"
#include "../solver/contradiction.h"
#include "../solver/enums.h"
#include "../solver/epq.h"
#include "../solver/grid.h"

#include "../solver/structs.h"

#include "loop.h"

/* Constructor takes a grid as input to generate a loop of desired length */
Loop::Loop(Grid & grid, int length, int m, int n) {
    
    grid_ = &grid;
    
    lengthMin_ = length;
    length_ = 0;
    m_ = m;
    n_ = n;
    
    srand(time(NULL));
    endPointI_ = (rand() % (m_-2)) + 2;
    endPointJ_ = (rand() % (n_-2)) + 2;
    
     
    
    makeLoop();
    clearNEdges();
    
    
}





/*  Begins to create loop */
void Loop::makeLoop() {
    srand (time(NULL));
    int direction = rand() % 4;
    
    if (direction == 0) {
        
        grid_->changeVLine(endPointI_ - 1, endPointJ_, LINE);
        attemptAdd(endPointI_ - 1, endPointJ_);
    } else if (direction == 1) {

        grid_->changeVLine(endPointI_, endPointJ_, LINE);
        attemptAdd(endPointI_ + 1, endPointJ_);
    } else if (direction == 2) {
        grid_->changeHLine(endPointI_, endPointJ_ - 1, LINE);
        attemptAdd(endPointI_, endPointJ_ - 1);
    } else if (direction == 3) {
        grid_->changeHLine(endPointI_, endPointJ_, LINE);
        attemptAdd(endPointI_, endPointJ_ + 1);
    }
    
}
               
/** Attepts to add an additional edge between the most recently 
  * added point and one that is adjacent to it **/
bool Loop::attemptAdd(int i, int j) {
    
    // If we are back at the starting point, then the loop is complete 
    if (i == endPointI_ && j == endPointJ_) {
        
        return true;
    }
    
    // Randomly selects the direction first attempted guess 
    srand (time(NULL));
    int direction = rand();
    int directions [5] = {direction % 4, (direction + 1) % 4, (direction + 2) % 4, (direction + 3) % 4, 5};
    for (int a = 0; a < 5; a++) {
        
        // Up
        if (directions[a] == 0) {
            i--;
            if (grid_->getVLine(i, j) == EMPTY) {
                if (i == endPointI_ && j == endPointJ_ && length_ <= lengthMin_) {
                    //Nothing? Don't want to "x" it out. 
                } else if (i != endPointI_ || j != endPointJ_) {
                    bool validAdd = checkUse(i, j);
                    if (validAdd) {
                        grid_->changeVLine(i, j, LINE);
                        length_++;
                        bool checkBeyond = attemptAdd(i, j);
                        if (checkBeyond) { 
                            return true; 
                        } else {
                            unmarkNEdges(i, j);
                            grid_->changeVLine(i, j, NLINE);
                            length_--;
                        }
                    } else {
                        grid_->changeVLine(i, j, NLINE);
                    }    
                } else {
                    grid_->changeVLine(i, j, LINE);
                    return true;
                } 
            }
            i++;
        }
        
        // Down
        else if (directions[a] == 1) {
            if (grid_->getVLine(i, j) == EMPTY) {
                if (i == endPointI_ && j == endPointJ_ && length_ <= lengthMin_) {
                    //Nothing? Don't want to "x" it out. 
                } else if (i != endPointI_ || j != endPointJ_) {
                    bool validAdd = checkUse(i + 1, j);
                    if (validAdd) {
                        grid_->changeVLine(i, j, LINE);
                        length_++;
                        bool checkBeyond = attemptAdd(i + 1, j); 
                        if (checkBeyond) { 
                            return true; 
                        } else {
                            
                            unmarkNEdges(i + 1, j);
                            grid_->changeVLine(i, j, NLINE);
                            length_--;
                        }
                    } else {
                        grid_->changeVLine(i, j, NLINE);
                    }    
                } else {
                    grid_->changeVLine(i, j, LINE);
                    return true;
                }
            }
        }
        
        // Left
        else if (directions[a] == 2) {
            j--;
            if (grid_->getHLine(i, j) == EMPTY) {
                if (i == endPointI_ && j == endPointJ_ && length_ <= lengthMin_) {
                    //Nothing? Don't want to "x" it out. 
                } else if (i != endPointI_ || j != endPointJ_) {
                    bool validAdd = checkUse(i, j);
                    if (validAdd) {
                        grid_->changeHLine(i, j, LINE);
                        length_++;
                        bool checkBeyond = attemptAdd(i, j); 
                        if (checkBeyond) { 
                            return true; 
                        } else {
                            unmarkNEdges(i, j);
                            grid_->changeHLine(i, j, NLINE);
                            length_--;
                        }
                    } else {
                        grid_->changeHLine(i, j, NLINE);
                    }    
                } else {
                    grid_->changeHLine(i, j, LINE);
                    return true;
                }
            }
            j++;
        }
        
        // Right
        else if (directions[a] == 3) {
            if (grid_->getHLine(i, j) == EMPTY) {
                if (i == endPointI_ && j == endPointJ_ && length_ <= lengthMin_) {
                    //Nothing? Don't want to "x" it out. 
                } else if (i != endPointI_ || j != endPointJ_) {
                    bool validAdd = checkUse(i, j + 1);
                    if (validAdd) {
                        grid_->changeHLine(i, j, LINE);
                        length_++;
                        bool checkBeyond = attemptAdd(i, j + 1); 
                        if (checkBeyond) {
                            return true; 
                        } else {
                            unmarkNEdges(i, j + 1);
                            length_--;
                            grid_->changeHLine(i, j, NLINE);
                        }
                    } else {
                        grid_->changeHLine(i, j, NLINE);
                    }    
                } else {
                    grid_->changeHLine(i, j, LINE);
                    return true;
                }
            }
        }
        
        else {
            return false;
        }  
    }  
}

/* Checks if a non-endpoint has any out-going edges.
If so, then cannot be used.*/
bool Loop::checkUse(int i, int j) {
    if (grid_->checkEdgeV(i, j) == LINE) {
        return false;
    }
    
    if (grid_->checkEdgeH(i, j) == LINE) {
        return false;
    }
    
    if (grid_->checkEdgeV(i - 1, j) == LINE) {

        return false;
    }
    
    if (grid_->checkEdgeH(i, j - 1) == LINE) {
        return false;
    }
    
    return true;
}

/** Clears all edges around a certain coordinate point. 
  * Will not clear if the edge is one denoting the grid boundary. **/
void Loop::unmarkEdges(int i, int j) {
    if (i < m_ + 1 && j < n_ + 1 ) {
        grid_->changeVLine(i, j, EMPTY);
        grid_->changeHLine(i, j, EMPTY);
    } if (i < m_ + 1 && j > n_) {
        grid_->changeVLine(i, j, EMPTY);
    } if (i > m_ && j < n_ + 1) {
        grid_->changeHLine(i, j, EMPTY);
    } 
    
    if (i - 1 > 0 ) {
        grid_->changeVLine(i - 1, j, EMPTY);
    }
    
    if (j - 1 > 0) {
        grid_->changeHLine(i, j - 1, EMPTY);
    }
}

/** Clears all negative edges from the grid once the loop is complete **/
void Loop::clearNEdges() {
    for (int i = 1; i < m_+1; i++) {
        for (int j = 1; j < n_+2; j++) {
            if (grid_->getVLine(i, j) == NLINE) {
                grid_->changeVLine(i, j, EMPTY);
            }
        }
    }
    
    for (int j = 1; j < n_+1; j++) {
        for (int i = 1; i < m_+2; i++) {
            if (grid_->getHLine(i, j) == NLINE) {
                grid_->changeHLine(i, j, EMPTY);
            }
        }
    }
}
    