TARGET    = run
SRC_DIR   = src
OBJ_DIR   = obj
INC_DIR   = include

CPP_FILES = $(wildcard $(SRC_DIR)/*.cpp)

OBJ_FILES = $(addprefix $(OBJ_DIR)/,$(notdir $(CPP_FILES:.cpp=.o)))

OBJS =  $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(notdir $(CPP_FILES)))

CXXFLAGS = -W -Wall -I$(INC_DIR) 
CFLAGS = -W -Wall -I$(INC_DIR) 
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


