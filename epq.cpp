#include "epq.h"
#include <cassert>
#include <queue>
#include "structs.h"

void EPQ::initEPQ(int m, int n)  {
    assert(m > 0 && n > 0);

    m_ = m;
    n_ = n;

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            pq.push(createPrioEdge(0, i, j, true)); 
            pq.push(createPrioEdge(0, i, j, false));
        }
        pq.push(createPrioEdge(0, i, n, false));
    }

    for (int j = 0; j < n; j++) {
        pq.push(createPrioEdge(0, m, j, true));
    }
}

PrioEdge EPQ::createPrioEdge(double prio, int i, int j, bool hLine) {
    assert(i >= 0 && (i - hLine) < m_ && j >= 0 && (j - !hLine) < n_);

    return PrioEdge { Coordinates { i, j }, prio, hLine };
}

bool EPQ::empty() const {
    return pq.empty();
}

int EPQ::size() const {
    return pq.size();
}

PrioEdge EPQ::top() const {
    return pq.top();
}

void EPQ::push(PrioEdge pe) {
    pq.push(pe);
}

void EPQ::pop() {
    pq.pop();
}

void EPQ::emplace(double prio, int i, int j, bool hLine) {
    pq.push(createPrioEdge(prio, i, j, hLine));
}
