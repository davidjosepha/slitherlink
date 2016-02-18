#include <iostream>
#include <time.h>
#include "generator.h"

int main(int argc, char * argv[]) {
    clock_t startTime, endTime;
    startTime = clock();
    int depth = 1;

    Generator g = Generator(10, 10); //one square for left/right top/bottom boundries

    
    endTime = clock();
    float diff = ((float)endTime - (float)startTime) / CLOCKS_PER_SEC;
    std::cout << "Time to solve:\t" << diff << " seconds" << std::endl;
    

    return 0;
}
