CC = clang++
CCFLAGS = -g

ifeq ($(OS),Windows_NT)
	CCFLAGS += -mwindows -std=c++11
else
	UNAME := $(shell uname)

	ifeq ($(UNAME),Linux)
		CCFLAGS += -std=gnu++11
	endif

	ifeq ($(UNAME),Darwin)
		CCFLAGS += -std=gnu++11
	endif
endif

SOLVERDEPS = solver/solver.cpp solver/epq.cpp solver/import.cpp solver/export.cpp solver/lattice.cpp solver/grid.cpp solver/contour.cpp solver/rotate.cpp solver/rules.cpp solver/rule.cpp solver/contradictions.cpp solver/contradiction.cpp

GENERATORDEPS = generator/generator.cpp

all: slsolver slgenerator

slsolver:
	$(CC) solver/main.cpp $(SOLVERDEPS) -o slsolver $(CCFLAGS)

slgenerator:
	$(CC) generator/main.cpp $(GENERATORDEPS) $(SOLVERDEPS) -o slgenerator $(CCFLAGS)

clean:
	rm slsolver slgenerator
