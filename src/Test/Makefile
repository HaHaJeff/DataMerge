CXX=g++
CC=gcc
C_FLAGS+=-g -std=c++11

OBJ_DIR=./
SRC_DIR=..
BIN_DIR=./

OBJECTS=$(OBJ_DIR)/*.o
PROGRAM=$(BIN_DIR)/Main

$(BIN_DIR)/Test: $(OBJ_DIR)/Test.o $(OBJ_DIR)/File.o $(OBJ_DIR)/Block.o $(OBJ_DIR)/Model.o
	@echo "Compiling $^ ==> $@..."
	$(CXX) $(C_FLAGS) $(OBJECTS) -o $(PROGRAM)

$(OBJ_DIR)/Test.o: Test.cc 
	@echo "Compiling $^ ==> $@..."
	$(CXX) $(C_FLAGS) -c $< -o $@

$(OBJ_DIR)/File.o: $(SRC_DIR)/File.cc
	@echo "Compiling $< ==> $@..."
	$(CXX) $(C_FLAGS) -c $< -o $@

$(OBJ_DIR)/Block.o: $(SRC_DIR)/Block.cc
	@echo "Compiling $< ==> $@..."
	$(CXX) $(C_FLAGS) -c $< -o $@

$(OBJ_DIR)/Model.o: $(SRC_DIR)/Model.cc
	@echo "Compiling $< ==> $@..."
	$(CXX) $(C_FLAGS) -c $< -o $@

.PHONY: clean


clean:
	rm $(PROGRAM) $(OBJECTS)
