#ifndef EPQ_H
#define EPQ_H
#include <queue>
#include <vector>
#include "structs.h"

class epq
{

	class ComparePrioEdge {
	public:
	    bool operator()(PrioEdge& e1, PrioEdge& e2)
	    {
	        if (e1.priority < e2.priority) return true;
	        return false;
	    }
	};

	public:
		epq() {};
		~epq() {};
		void initEPQ(int m, int n);
		PrioEdge createPrioEdge(double prio, int m, int n, bool hLine);

		bool empty();
		int size();
		PrioEdge top();
		void push(PrioEdge pe);
		void pop();
		void emplace(double prio, int m, int n, bool hLine);
		void swap(epq ep2);
	
		// void setPriority(double prio) {priority = prio;};
		// void setM(int i) {m = i;};
		// void setN(int i) {n = i;};
		// void setH(bool b) {h = b;};
		// void setV(bool b) {h = !b;};
		// double getPriority() {return priority;};
		// int getM() {return m;};
		// int getN() {return n};
		// bool getH() {return h;};
		// bool getV() {return !h;};

	protected:
		int m_;
		int n_;
		std::priority_queue<PrioEdge, std::vector<PrioEdge>, ComparePrioEdge> pq;
};

#endif
