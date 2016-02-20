#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <time.h>
#include "generator.h"

int main(int argc, char * argv[]) {
    
    clock_t startTime, endTime;
    
    
    if (argc == 3) {
        
        std::istringstream mn(argv[1]);
        std::istringstream nn(argv[2]);
        int m, n;
        assert(mn >> m && mn.eof());
        assert(nn >> n && nn.eof());
        
        startTime = clock();
        Generator g = Generator(m, n, HARD); //one square for left/right top/bottom boundries

        endTime = clock();
        float diff = ((float)endTime - (float)startTime) / CLOCKS_PER_SEC;
        std::cout << "Time to solve:\t" << diff << " seconds" << std::endl;
        return 0;
        
    } 
    
}
