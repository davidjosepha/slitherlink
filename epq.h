#ifndef EPQ_H
#define EPQ_H
#include <queue>
#include <vector>
#include "structs.h"

class epq {
    class ComparePrioEdge {
        public:
            bool operator()(PrioEdge & e1, PrioEdge & e2) const { return e1.priority < e2.priority; };
    };

    public:
        epq() {};
        void initEPQ(int m, int n);
        PrioEdge createPrioEdge(double prio, int i, int j, bool hLine);

        bool empty();
        int size();
        PrioEdge top();
        void push(PrioEdge pe);
        void pop();
        void emplace(double prio, int i, int j, bool hLine);
        void swap(epq ep2);

    protected:
        int m_;
        int n_;
        std::priority_queue<PrioEdge, std::vector<PrioEdge>, ComparePrioEdge> pq;
};

#endif
