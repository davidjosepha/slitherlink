CC = clang++
CCFLAGS = -g -std=gnu++11

SOLVER_SOURCES := $(filter-out src/solver/main.cpp, $(wildcard src/solver/*.cpp))
GENERATOR_SOURCES := $(filter-out src/generator/main.cpp, $(wildcard src/generator/*.cpp))

SOLVER_OBJECTS = $(SOLVER_SOURCES:.cpp=.o)
GENERATOR_OBJECTS = $(GENERATOR_SOURCES:.cpp=.o)

all: slsolver slgenerator

slsolver: $(SOLVER_OBJECTS)
	$(CC) $(CCFLAGS) src/solver/main.cpp $^ -o $@

slgenerator: $(SOLVER_OBJECTS) $(GENERATOR_OBJECTS)
	$(CC) $(CCFLAGS) src/generator/main.cpp $^ -o $@

#obj/%.o: src/%.cpp
.cpp.o:
	$(CC) -c $(CCFLAGS) $< -o $@

clean:
	rm slsolver slgenerator src/solver/*.o src/generator/*.o
