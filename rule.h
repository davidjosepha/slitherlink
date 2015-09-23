#ifndef RULE_H
#define RULE_H
#include "enums.h"

class Rule {
    public:
        Rule();
        ~Rule();

    private:
        int m;     // number of rows
        int n;     // number of columns
        Number ** numbers;
        Edge ** hlines;
        Edge ** vlines;
};

#endif
