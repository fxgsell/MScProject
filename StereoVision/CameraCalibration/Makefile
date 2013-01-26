NAME=CameraCalibration
CXX=g++
CXXFLAGS=`pkg-config opencv --cflags`
LDFLAGS=`pkg-config opencv --libs`
SRCS=main.cpp

all: $(NAME)

$(NAME): 
	$(CXX) $(SRCS) $(LDFLAGS) $(CXXFLAGS) -o $(NAME)

clean:
	rm -f $(NAME) 

re: clean all
