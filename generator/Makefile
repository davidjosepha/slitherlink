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

all:
	$(CC) main.cpp generator.cpp ../solver/solver.cpp ../solver/epq.cpp ../solver/import.cpp ../solver/export.cpp ../solver/lattice.cpp ../solver/grid.cpp ../solver/contour.cpp ../solver/rotate.cpp ../solver/rules.cpp ../solver/rule.cpp ../solver/contradictions.cpp ../solver/contradiction.cpp -o slgenerator $(CCFLAGS)
clean:
	rm *.o slitherlink
