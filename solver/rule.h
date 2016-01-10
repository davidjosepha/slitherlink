#ifndef RULE_H
#define RULE_H
#include <vector>
#include "enums.h"
#include "lattice.h"
#include "structs.h"

class Rule {
    public:
        Rule() { };
        Rule(int m, int n);
        int getHeight() const { return m_; };
        int getWidth() const { return n_; };

        std::vector<NumberPosition> const * getNumberPattern() const { return &numberPattern_; }
        std::vector<EdgePosition> const * getHLinePattern() const { return &hLinePattern_; }
        std::vector<EdgePosition> const * getVLinePattern() const { return &vLinePattern_; }
        std::vector<EdgePosition> const * getHLineDiff() const { return &hLineDiff_; }
        std::vector<EdgePosition> const * getVLineDiff() const { return &vLineDiff_; }

        int getNumberHeight(Orientation orient) const;
        int getNumberWidth(Orientation orient) const;

        void addNumberPattern(int i, int j, Number num) {
            numberPattern_.push_back(NumberPosition { Coordinates { i, j }, num });
        };
        void addHLinePattern(int i, int j, Edge edge) {
            hLinePattern_.push_back(EdgePosition { Coordinates { i, j }, edge });
        };
        void addVLinePattern(int i, int j, Edge edge) {
            vLinePattern_.push_back(EdgePosition { Coordinates { i, j }, edge });
        };
        void addHLineDiff(int i, int j, Edge edge) {
            hLineDiff_.push_back(EdgePosition { Coordinates { i, j }, edge });
        };
        void addVLineDiff(int i, int j, Edge edge) {
            vLineDiff_.push_back(EdgePosition { Coordinates { i, j }, edge });
        };

    private:
        int m_;
        int n_;

        std::vector<NumberPosition> numberPattern_;
        std::vector<EdgePosition> hLinePattern_;
        std::vector<EdgePosition> vLinePattern_;
        std::vector<EdgePosition> hLineDiff_;
        std::vector<EdgePosition> vLineDiff_;
};

#endif
