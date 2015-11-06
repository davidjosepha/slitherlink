#include "epq.h"
#include <cassert>
#include <queue>
#include "structs.h"

void epq::initEPQ(int m, int n)  {
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

PrioEdge epq::createPrioEdge(double prio, int i, int j, bool hLine) {
    assert(i >= 0 && (i - hLine) < m_ && j >= 0 && (j - !hLine) < n_);

    return PrioEdge { Coordinates { i, j }, prio, hLine };
}

bool epq::empty() const {
    return pq.empty();
}

int epq::size() const {
    return pq.size();
}

PrioEdge epq::top() const {
    return pq.top();
}

void epq::push(PrioEdge pe) {
    pq.push(pe);
}

void epq::pop() {
    pq.pop();
}

void epq::emplace(double prio, int i, int j, bool hLine) {
    pq.push(createPrioEdge(prio, i, j, hLine));
}
