#include "rule.h"
#include "enums.h"

Rule::Rule() {
    m = 10;
    n = 10;

    numbers = new Number*[m];
    hlines = new Edge*[m+1];
    vlines = new Edge*[m];
    for (int i = 0; i < m; i++) {
        numbers[i] = new Number[n];
        hlines[i] = new Edge[n];
        vlines[i] = new Edge[n+1];
    }
    // hlines needs one extra
    hlines[m] = new Edge[n];
}

Rule::~Rule() {
    for (int i = 0; i < m; i++) {
        delete [] numbers[i], hlines[i], vlines[i];
    }
    // hlines needs one extra
    delete [] hlines[m];
    // and delete the outer arrays
    delete [] numbers, hlines, vlines;
}
