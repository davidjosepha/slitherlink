#include <iostream>
#include <fstream>
#include <string>
#include <time.h>

int main(int argc, char * argv[]) {
    clock_t startTime, endTime;
    startTime = clock();
    int depth = 1;

    endTime = clock();
    float diff = ((float)endTime - (float)startTime) / CLOCKS_PER_SEC;
    std::cout << "Time to solve:\t" << diff << " seconds" << std::endl;

    return 0;
}
