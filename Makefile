CXX 		:= g++
CXX_FLAGS	:= -ggdb

BIN 		:= bin
SRC			:= src
SRC_MAIN 	:= src/main
SRC_TEST	:= src/test
INCLUDE		:= include

LIBRARIES 	:= 
EXECUTABLE 	:= main
TEST		:= test

all: $(BIN)/$(EXECUTABLE)

run: clean all
	clear
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/main.cpp $(SRC_MAIN)/*.cpp
	mkdir -p $(BIN)
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) $^ -o $@ $(LIBRARIES)

test: clean $(BIN)/$(TEST)
	clear
	./$(BIN)/$(TEST)

$(BIN)/$(TEST): $(SRC)/test.cpp $(SRC_MAIN)/*.cpp
	mkdir -p $(BIN)
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) $^ -o $@ $(LIBRARIES)	

clean:
	-rm -f $(BIN)/*