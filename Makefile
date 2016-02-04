CC = clang++
CCFLAGS = -g -std=gnu++11

OBJ_DIR := obj
SRC_DIR := src
SOLVER_DIR := $(SRC_DIR)/solver
GENERATOR_DIR := $(SRC_DIR)/generator

SOLVER_EXEC := slsolver
GENERATOR_EXEC := slgenerator

SOLVER_SOURCES := $(filter-out $(SOLVER_DIR)/main.cpp, $(wildcard $(SOLVER_DIR)/*.cpp))
GENERATOR_SOURCES := $(filter-out $(GENERATOR_DIR)/main.cpp, $(wildcard $(GENERATOR_DIR)/*.cpp))
SOLVER_MAIN := $(SOLVER_DIR)/main.cpp
GENERATOR_MAIN := $(GENERATOR_DIR)/main.cpp

SOLVER_OBJECTS := $(addprefix $(OBJ_DIR)/, $(notdir $(SOLVER_SOURCES:.cpp=.o)))
GENERATOR_OBJECTS := $(addprefix $(OBJ_DIR)/, $(notdir $(GENERATOR_SOURCES:.cpp=.o)))
SOLVER_MAIN_O := $(addprefix $(OBJ_DIR)/, main_solver.o)
GENERATOR_MAIN_O := $(addprefix $(OBJ_DIR)/, main_generator.o)

all: directories $(SOLVER_EXEC) $(GENERATOR_EXEC)

directories: $(OBJ_DIR)

$(SOLVER_EXEC): $(SOLVER_OBJECTS) $(SOLVER_MAIN_O)
	$(CC) $(CCFLAGS) $^ -o $@

$(GENERATOR_EXEC): $(SOLVER_OBJECTS) $(GENERATOR_OBJECTS) $(GENERATOR_MAIN_O)
	$(CC) $(CCFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SOLVER_DIR)/%.cpp $(SOLVER_DIR)/%.h
	$(CC) -c $(CCFLAGS) $< -o $@

$(OBJ_DIR)/%.o: $(GENERATOR_DIR)/%.cpp $(GENERATOR_DIR)/%.h
	$(CC) -c $(CCFLAGS) $< -o $@

$(SOLVER_MAIN_O): $(SOLVER_MAIN)
	$(CC) -c $(CCFLAGS) $< -o $@

$(GENERATOR_MAIN_O): $(GENERATOR_MAIN)
	$(CC) -c $(CCFLAGS) $< -o $@

$(OBJ_DIR):
	mkdir -p $@

clean:
	rm $(SOLVER_EXEC) $(GENERATOR_EXEC) $(OBJ_DIR)/*.o
