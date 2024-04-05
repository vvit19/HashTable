CFLAGS = -g -O3 -fno-omit-frame-pointer

СXX = g++
TARGET = main
IFLAGS = -I./include/

SRC_FOLDER = ./src/
OBJ_FOLDER = ./obj/

SRC = $(wildcard $(SRC_FOLDER)*.cpp)
OBJ = $(patsubst $(SRC_FOLDER)%.cpp, $(OBJ_FOLDER)%.o, $(SRC))

$(TARGET) : $(OBJ)
	@$(CXX) $(IFLAGS) $(CFLAGS) $(OBJ) -o $(TARGET)

$(OBJ_FOLDER)%.o : $(SRC_FOLDER)%.cpp
	@mkdir -p $(@D)
	@$(CXX) -mavx2 $(IFLAGS) $(CFLAGS) -c $< -o $@

clean:
	rm $(TARGET) $(OBJ)

draw:
	dot -v -Tpng graph.dot -o graph.png
