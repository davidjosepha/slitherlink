#include "epq.h"
#include <cassert>
#include <queue>
#include "../shared/structs.h"

void EPQ::initEPQ(int m, int n)  {
    assert(m > 0 && n > 0);

    m_ = m;
    n_ = n;

    for (int i = 1; i < m-1; i++) {
        for (int j = 1; j < n-1; j++) {
            pq_.push(createPrioEdge(0, i, j, true));
            pq_.push(createPrioEdge(0, i, j, false));
        }
        pq_.push(createPrioEdge(0, i, n-1, false));
    }

    for (int j = 1; j < n-1; j++) {
        pq_.push(createPrioEdge(0, m-1, j, true));
    }
}

PrioEdge EPQ::createPrioEdge(double prio, int i, int j, bool hLine) {
    assert(i >= 0 && (i - hLine) < m_ && j >= 0 && (j - !hLine) < n_);

    return PrioEdge { Coordinates { i, j }, prio, hLine };
}

bool EPQ::empty() const {
    return pq_.empty();
}

int EPQ::size() const {
    return pq_.size();
}

PrioEdge EPQ::top() const {
    return pq_.top();
}

void EPQ::push(PrioEdge pe) {
    pq_.push(pe);
}

void EPQ::pop() {
    pq_.pop();
}

void EPQ::emplace(double prio, int i, int j, bool hLine) {
    pq_.push(createPrioEdge(prio, i, j, hLine));
}
// class ComparePrioEdge {
//     public:
//         bool operator()(PrioEdge & e1, PrioEdge & e2) const { return e1.priority < e2.priority; }
// };

std::vector<PrioEdge> EPQ::copyPQToVector(){
    std::priority_queue<PrioEdge, std::vector<PrioEdge>, ComparePrioEdge> newPQ;
    std::vector<PrioEdge> outputvec(pq_.size());
    std::copy(&(pq_.top()), &(pq_.top()) + pq_.size(), &outputvec[0]);
    return outputvec;
}

void EPQ::copyPQ(EPQ orig) {
    std::vector<PrioEdge> prioEdgeVec = orig.copyPQToVector();
    for (int i = 0; i < prioEdgeVec.size(); i++) {
        pq_.push(prioEdgeVec[i]);
    }
}

void EPQ::copySubsetPQ(EPQ orig) {
    PrioEdge pe = orig.top();
    std::vector<PrioEdge> prioEdgeVec = orig.copyPQToVector();
    for (int i = 0; i < prioEdgeVec.size(); i++) {
        PrioEdge cur = prioEdgeVec[i];
        if (cur.coords.i < pe.coords.i && cur.coords.j < pe.coords.j)
            pq_.push(prioEdgeVec[i]);
    }
}





