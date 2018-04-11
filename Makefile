CXX=g++
CC=gcc
C_FLAGS+=-g -std=c++11

OBJ_DIR=obj
SRC_DIR=src
BIN_DIR=bin

OBJECTS=$(OBJ_DIR)/Main.o $(OBJ_DIR)/Model.o  $(OBJ_DIR)/File.o $(OBJ_DIR)/Block.o
PROGRAM=$(BIN_DIR)/Main

$(BIN_DIR)/Main: $(OBJ_DIR)/Main.o $(OBJ_DIR)/Model.o $(OBJ_DIR)/File.o $(OBJ_DIR)/Block.o
	@echo "Compiling $^ ==> $@..."
	$(CXX) $(C_FLAGS) $(OBJECTS) -o $(PROGRAM)

$(OBJ_DIR)/Main.o: $(SRC_DIR)/Main.cc 
	@echo "Compiling $^ ==> $@..."
	$(CXX) $(C_FLAGS) -c $< -o $@

$(OBJ_DIR)/Model.o: $(SRC_DIR)/Model.cc
	@echo "Compiling $< ==> $@..."
	$(CXX) $(C_FLAGS) -c $< -o $@

$(OBJ_DIR)/File.o: $(SRC_DIR)/File.cc
	@echo "Compiling $< ==> $@..."
	$(CXX) $(C_FLAGS) -c $< -o $@

$(OBJ_DIR)/Block.o: $(SRC_DIR)/Block.cc
	@echo "Compiling $< ==> $@..."
	$(CXX) $(C_FLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm $(PROGRAM) $(OBJECTS)
