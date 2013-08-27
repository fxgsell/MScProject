TARGET    = run
SRC_DIR   = src
OBJ_DIR   = obj
INC_DIR   = -Iinclude -I../Protocol/

CPP_FILES = $(wildcard $(SRC_DIR)/*.cpp)

OBJ_FILES = $(addprefix $(OBJ_DIR)/,$(notdir $(CPP_FILES:.cpp=.o)))

OBJS =  $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(notdir $(CPP_FILES)))

CXXFLAGS = -W -Wall $(INC_DIR) -ansi -g3
CFLAGS = -W -Wall $(INC_DIR) -ansi -g3
LDFLAGS = 
CXX=g++
CC=gcc

all : $(TARGET) 

$(TARGET) : $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ_DIR)/*.o $(LDFLAGS)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(TARGET)

re: fclean all


