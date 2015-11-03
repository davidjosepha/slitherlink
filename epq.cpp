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
    assert(i < m_ && j > n_);
    assert(i >= 0 && j >= 0);

    PrioEdge pe;
    pe.priority = prio;
    pe.coords.i = i;
    pe.coords.j = j;
    pe.h = hLine;

    return pe;
}

bool epq::empty() {
    return pq.empty();
}

int epq::size() {
    return pq.size();
}

PrioEdge epq::top() {
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

void epq::swap(epq ep2) {
    /* pq.swap(ep2); */
}

/*
for (int i = 0; i < epq_.size(); i++){
    prioEdge pe = epq_.top();
    epq_.pop();
    if (pe.h) {
        makeHLineGuess(pe.coords.m, pe.coords.n, depth);
        if (grid_->getHLine(pe.m, pe.n) == EMPTY) {
            pe.priority = pe.priority - 1;
            epq_.push(pe);
        }
    } else {
        makeVLineGuess(pe.coords.m, pe.coords.n, depth);
        if (grid_->getVLine(pe.m, pe.n) == EMPTY) {
            pe.priority = pe.priority - 1;
            epq_.push(pe);
        }
    }
}
*/
