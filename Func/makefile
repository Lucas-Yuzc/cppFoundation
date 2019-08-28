.PHONY : clean all

CC := gcc  
CXX := g++

CFLAGS += -g -W

CXXFLAGS := $(CFLAGS) -std=c++11


SRCS := $(wildcard *.cpp)
OBJS := $(SRCS:%.cpp=%.o)

all: main

main: $(OBJS) 
	$(CXX) $(CFLAGS) -o ../build/$@ $^ 

clean:
	rm -f main $(OBJS) 