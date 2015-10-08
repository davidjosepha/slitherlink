#include "lattice.h"
#include <vector>
#include "contour.h"

class Grid : public Lattice {
    public:
        Grid() { };

    private:
        void mergeContours();

        std::vector<Contour> contours_;
};
