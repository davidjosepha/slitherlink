#include <iostream>
#include "export.h"
#include "import.h"
#include "lattice.h"

int main(int argc, char * argv[]) {
    Lattice grid;
    Import importer = Import(grid);
    Export exporter = Export(grid);
    exporter.print();
    return 0;
}
