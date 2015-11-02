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
	$(CC) main.cpp solver.cpp import.cpp export.cpp lattice.cpp grid.cpp contour.cpp rotate.cpp rules.cpp rule.cpp contradictions.cpp contradiction.cpp -o slitherlink $(CCFLAGS)
clean:
	rm *.o slitherlink
