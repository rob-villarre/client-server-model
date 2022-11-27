#Directories
SRC_DIR =./# ./src#.cpp files
BUILD_DIR =./# ./build#object files
INCLUDE_DIR =./# ./inc#.hpp files
BIN_DIR =./# ./bin#exacutable

# valgrind --leak-check=yes --leak-check=full --show-leak-kinds=all ./bin/run

SERVER_OBJS = $(BUILD_DIR)/server_main.o $(BUILD_DIR)/server.o $(BUILD_DIR)/tands.o
CLIENT_OBJS = $(BUILD_DIR)/client_main.o $(BUILD_DIR)/client.o $(BUILD_DIR)/tands.o
SERVER_LINK_TARGET = $(BIN_DIR)/server
CLIENT_LINK_TARGET = $(BIN_DIR)/client

CC = g++
COMPILER_FLAGS = -O -g -w -Wall -Wextra

#Note: $< is the first prerequisite
#Note: $@ name of target

all: $(SERVER_LINK_TARGET) $(CLIENT_LINK_TARGET)

$(SERVER_LINK_TARGET): $(SERVER_OBJS) 
	$(CC) $(SERVER_OBJS)  $(COMPILER_FLAGS) -o $(SERVER_LINK_TARGET)

$(CLIENT_LINK_TARGET): $(CLIENT_OBJS) 
	$(CC) $(CLIENT_OBJS)  $(COMPILER_FLAGS) -o $(CLIENT_LINK_TARGET)

$(BUILD_DIR)/server_main.o: $(SRC_DIR)/server_main.cpp
	$(CC) $(COMPILER_FLAGS) -c $< -o $@

$(BUILD_DIR)/server.o: $(SRC_DIR)/server.cpp $(INCLUDE_DIR)/server.h
	$(CC) $(COMPILER_FLAGS) -c $< -o $@

$(BUILD_DIR)/tands.o: $(SRC_DIR)/tands.cpp $(INCLUDE_DIR)/tands.h
	$(CC) $(COMPILER_FLAGS) -c $< -o $@

$(BUILD_DIR)/client_main.o: $(SRC_DIR)/client_main.cpp
	$(CC) $(COMPILER_FLAGS) -c $< -o $@

$(BUILD_DIR)/client.o: $(SRC_DIR)/client.cpp $(INCLUDE_DIR)/client.h
	$(CC) $(COMPILER_FLAGS) -c $< -o $@

clean:
	rm -f $(BUILD_DIR)/*.o
	rm -f $(LINK_TARGET)

serverman:
	groff -Tpdf -man < server.man > server.pdf

clientman:
	groff -Tpdf -man < client.man > client.pdf