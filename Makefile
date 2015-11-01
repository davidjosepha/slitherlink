CC = g++
CCFLAGS = -g

ifeq ($(OS),Windows_NT)
	# Windows flags, don't yet include gsl
	CCFLAGS += -mwindows -std=c++11
else
	UNAME := $(shell uname)

	ifeq ($(UNAME),Linux)
		CCFLAGS += -std=gnu++11
	endif

	# Mac flags, don't yet include gsl
	ifeq ($(UNAME),Darwin)
		#CCFLAGS += -framework GLUT -framework OPENGL
	endif
endif

all:
	$(CC) main.cpp solver.cpp import.cpp export.cpp lattice.cpp grid.cpp contour.cpp rules.cpp rule.cpp contradiction.cpp -o slitherlink $(CCFLAGS)
clean:
	rm *.o slitherlink
