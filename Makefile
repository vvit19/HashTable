CFLAGS = -g -O3 -mavx2 \
# -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr,

СXX = g++
TARGET = main
IFLAGS = -I./include/

SRC_FOLDER = ./src/
OBJ_FOLDER = ./obj/

SRC = $(wildcard $(SRC_FOLDER)*.cpp)
OBJ = $(patsubst $(SRC_FOLDER)%.cpp, $(OBJ_FOLDER)%.o, $(SRC))

$(TARGET) : $(OBJ) $(OBJ_FOLDER)mystrcmp.o
	@$(CXX) $(IFLAGS) $(CFLAGS) $^ -o $(TARGET)

$(OBJ_FOLDER)%.o : $(SRC_FOLDER)%.cpp
	@$(CXX) $(IFLAGS) $(CFLAGS) -c $^ -o $@

$(OBJ_FOLDER)mystrcmp.o : $(SRC_FOLDER)mystrcmp.s
	@mkdir -p $(@D)
	@nasm -f elf64 -g $(SRC_FOLDER)mystrcmp.s -o $(OBJ_FOLDER)mystrcmp.o

clean:
	@rm $(TARGET) $(OBJ_FOLDER)*.o
