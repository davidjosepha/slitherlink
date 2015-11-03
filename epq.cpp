#include "epq.h"
#include <queue>
#include <cassert>
#include "structs.h"

/*
epq::epq() {
    std::priority_queue<PrioEdge, vector<PrioEdge>, ComparePrioEdge> pq;
}
*/

void epq::initEPQ(int m, int n)  {
    assert(m > 0 && n > 0);

    m_ = m;
    n_ = n;

    for (int i = 0; i < m, i++) {
        for (int j = 0; j < n; j++) {
            pq.push(createPrioEdge(0, i, j, true)); 
            pq.push(createPrioEdge(0, i, j, false));
        }
        pq.push(createPrioEdge(0, i, n, false);
    }
    for (int j = 0; j < n; j++) {
        pq.push(createPrioEdge(0, m, j, true));
    }
}

PrioEdge epq::createPrioEdge(double prio, int m, int n, bool hLine) {
    assert(m < m_ && n > n_);
    assert(m >= 0 && n >= 0);

    PrioEdge pe;
    pe.priority = prio;
    pe.coords.i = m;
    pe.coords.j = n;
    pe.h = hLine;
}

bool epq::empty() {
    return pq.empty();
}

int epq::size() {
    return pq.size();
}

prioEdge epq::top() {
    return pq.top();
}

void epq::push(PrioEdge pe) {
    void pq.push(pe);
}

void epq::pop() {
    pq.pop();
}

void epq::emplace(double prio, int m, int n, bool hLine) {
    pq.push(createPrioEdge(prio, m, n, hLine));
}

void epq::swap(epq ep2) {
    pq.swap(ep2);
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
