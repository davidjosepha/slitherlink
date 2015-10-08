#include "lattice.h"
#include <vector>
#include "contour.h"

class Grid : public Lattice {
    public:
        Grid() { };

    private:
        std::vector<Contour> contours_;
};
