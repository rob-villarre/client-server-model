#Directories
SRC_DIR =./# ./src#.cpp files
BUILD_DIR =./# ./build#object files
INCLUDE_DIR =./# ./inc#.hpp files
BIN_DIR =./# ./bin#exacutable

# valgrind --leak-check=yes --leak-check=full --show-leak-kinds=all ./bin/run

OBJS = $(BUILD_DIR)/main.o $(BUILD_DIR)/client.o $(BUILD_DIR)/tands.o
LINK_TARGET = $(BIN_DIR)/client

CC = g++
COMPILER_FLAGS = -O -w -Wall

#Note: $< is the first prerequisite
#Note: $@ name of target

all: $(LINK_TARGET)

$(LINK_TARGET): $(OBJS)
	$(CC) $(OBJS)  $(COMPILER_FLAGS) -o $(LINK_TARGET)

$(BUILD_DIR)/main.o: $(SRC_DIR)/main.cpp
	$(CC) $(COMPILER_FLAGS) -c $< -o $@

$(BUILD_DIR)/client.o: $(SRC_DIR)/client.cpp $(INCLUDE_DIR)/client.h
	$(CC) $(COMPILER_FLAGS) -c $< -o $@

$(BUILD_DIR)/tands.o: $(SRC_DIR)/tands.cpp $(INCLUDE_DIR)/tands.h
	$(CC) $(COMPILER_FLAGS) -c $< -o $@

clean:
	rm -f $(BUILD_DIR)/*.o
	rm -f $(LINK_TARGET)