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
    
    if (argc == 4) {
        std::istringstream mn(argv[1]);
        std::istringstream nn(argv[2]);
        std::istringstream difficn(argv[3]);
        int m, n;
        std::string difficstr;
        assert(mn >> m && mn.eof());
        assert(nn >> n && nn.eof());
        assert(difficn >> difficstr && difficn.eof());

        Difficulty diffic = (difficstr == "e") ? EASY : HARD;

        startTime = clock();
        Generator g = Generator(m, n, diffic); //one square for left/right top/bottom boundries

        endTime = clock();
        float diff = ((float)endTime - (float)startTime) / CLOCKS_PER_SEC;
        std::cout << "Time to create:\t" << diff << " seconds" << std::endl;

        return EXIT_SUCCESS;
    } 
}
