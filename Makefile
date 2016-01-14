CC = clang++
CCFLAGS = -g -std=gnu++11

SOLVER_SOURCES := $(filter-out src/solver/main.cpp, $(wildcard src/solver/*.cpp))
GENERATOR_SOURCES := $(filter-out src/generator/main.cpp, $(wildcard src/generator/*.cpp))

SOLVER_OBJECTS := $(addprefix obj/, $(notdir $(SOLVER_SOURCES:.cpp=.o)))
GENERATOR_OBJECTS := $(addprefix obj/, $(notdir $(GENERATOR_SOURCES:.cpp=.o)))

all: directories slsolver slgenerator

directories: obj

slsolver: $(SOLVER_OBJECTS)
	$(CC) $(CCFLAGS) src/solver/main.cpp $^ -o $@

slgenerator: $(SOLVER_OBJECTS) $(GENERATOR_OBJECTS)
	$(CC) $(CCFLAGS) src/generator/main.cpp $^ -o $@

obj/%.o: src/solver/%.cpp
	$(CC) -c $(CCFLAGS) $< -o $@

obj/%.o: src/generator/%.cpp
	$(CC) -c $(CCFLAGS) $< -o $@

obj:
	mkdir -p $@

clean:
	rm slsolver slgenerator obj/*.o
