CC = clang++
CCFLAGS = -g -std=gnu++11

SOLVER_SOURCES = solver/lattice.cpp solver/solver.cpp solver/epq.cpp solver/import.cpp solver/export.cpp solver/grid.cpp solver/contour.cpp solver/rotate.cpp solver/rules.cpp solver/rule.cpp solver/contradictions.cpp solver/contradiction.cpp
GENERATOR_SOURCES = generator/generator.cpp

SOLVER_OBJECTS = $(SOLVER_SOURCES:.cpp=.o)
GENERATOR_OBJECTS = $(GENERATOR_SOURCES:.cpp=.o)

all: slsolver slgenerator

slsolver: $(SOLVER_OBJECTS)
	$(CC) $(CCFLAGS) solver/main.cpp $^ -o $@

slgenerator: $(SOLVER_OBJECTS) $(GENERATOR_OBJECTS)
	$(CC) $(CCFLAGS) generator/main.cpp $^ -o $@

.cpp.o:
	$(CC) -c $(CCFLAGS) $< -o $@

clean:
	rm slsolver slgenerator solver/*.o generator/*.o
