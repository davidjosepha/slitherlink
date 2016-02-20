#ifndef EPQ_H
#define EPQ_H
#include <queue>
#include <vector>
#include "../shared/structs.h"

class EPQ {
    class ComparePrioEdge {
        public:
            bool operator()(PrioEdge & e1, PrioEdge & e2) const { return e1.priority < e2.priority; };
    };

    public:
        EPQ() { };
        void initEPQ(int m, int n);
        PrioEdge createPrioEdge(double prio, int i, int j, bool hLine);

        bool empty() const;
        int size() const;
        PrioEdge top() const;
        void push(PrioEdge pe);
        void pop();
        void emplace(double prio, int i, int j, bool hLine);
        std::vector<PrioEdge> copyPQToVector() const;
        void copyPQ(EPQ orig);
        void copySubsetPQ(EPQ orig);

    protected:
        int m_;
        int n_;
        std::priority_queue<PrioEdge, std::vector<PrioEdge>, ComparePrioEdge> pq_;
};

#endif
