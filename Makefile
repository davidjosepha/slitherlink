CC = clang++
CCFLAGS = -g -std=gnu++11

SOLVER_SOURCES := $(filter-out src/solver/main.cpp, $(wildcard src/solver/*.cpp))
GENERATOR_SOURCES := $(filter-out src/generator/main.cpp, $(wildcard src/generator/*.cpp))
SOLVER_MAIN := src/solver/main.cpp
GENERATOR_MAIN := src/generator/main.cpp

SOLVER_OBJECTS := $(addprefix obj/, $(notdir $(SOLVER_SOURCES:.cpp=.o)))
GENERATOR_OBJECTS := $(addprefix obj/, $(notdir $(GENERATOR_SOURCES:.cpp=.o)))
SOLVER_MAIN_O := $(addprefix obj/, main_solver.o)
GENERATOR_MAIN_O := $(addprefix obj/, main_generator.o)

all: directories slsolver slgenerator

directories: obj

slsolver: $(SOLVER_OBJECTS) $(SOLVER_MAIN_O)
	$(CC) $(CCFLAGS) $^ -o $@

slgenerator: $(SOLVER_OBJECTS) $(GENERATOR_OBJECTS) $(GENERATOR_MAIN_O)
	$(CC) $(CCFLAGS) $^ -o $@

obj/%.o: src/solver/%.cpp
	$(CC) -c $(CCFLAGS) $< -o $@

obj/%.o: src/generator/%.cpp
	$(CC) -c $(CCFLAGS) $< -o $@

$(SOLVER_MAIN_O): $(SOLVER_MAIN)
	$(CC) -c $(CCFLAGS) $< -o $@

$(GENERATOR_MAIN_O): $(GENERATOR_MAIN)
	$(CC) -c $(CCFLAGS) $< -o $@

obj:
	mkdir -p $@

clean:
	rm slsolver slgenerator obj/*.o
