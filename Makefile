CFLAGS = -g -O3 -mavx2 \

СXX = g++
TARGET = main
IFLAGS = -I./include/

SRC_FOLDER = ./src/
OBJ_FOLDER = ./obj/

SRC = $(wildcard $(SRC_FOLDER)*.cpp)
OBJ = $(patsubst $(SRC_FOLDER)%.cpp, $(OBJ_FOLDER)%.o, $(SRC))

$(TARGET) : $(OBJ) $(OBJ_FOLDER)crchash.o
	@$(CXX) $(IFLAGS) $(CFLAGS) $^ -o $(TARGET)

$(OBJ_FOLDER)%.o : $(SRC_FOLDER)%.cpp $(OBJ_FOLDER)crchash.o
	@$(CXX) $(IFLAGS) $(CFLAGS) -c $^ -o $@

$(OBJ_FOLDER)crchash.o : $(SRC_FOLDER)crchash.s
	@mkdir -p $(@D)
	@nasm -f elf64 -g $(SRC_FOLDER)crchash.s -o $(OBJ_FOLDER)crchash.o

clean:
	@rm $(TARGET) $(OBJ_FOLDER)*.o
