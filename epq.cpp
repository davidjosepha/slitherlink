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
            pq_.push(createPrioEdge(0, i, j, true)); 
            pq_.push(createPrioEdge(0, i, j, false));
        }
        pq_.push(createPrioEdge(0, i, n, false));
    }

    for (int j = 0; j < n; j++) {
        pq_.push(createPrioEdge(0, m, j, true));
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
    
std::vector<PrioEdge> EPQ::copyToVector(){
    std::priority_queue<PrioEdge, std::vector<PrioEdge>, ComparePrioEdge> newPQ;
    std::vector<PrioEdge> outputvec(pq_.size());
    std::copy(&(pq_.top()), &(pq_.top()) + pq_.size(), &outputvec[0]);
    return outputvec;
}





