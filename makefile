CC = g++
CFLAGS = -g -Wall
BINS = main
SRC_DIRS := src/lib

CHECKCC = valgrind
CHECKFLAGS = --leak-check=full -s --show-leak-kinds=all

SRCS := $(wildcard $(SRC_DIRS:=/*.cpp))
OBJS := $(SRCS:.cpp=.o)

all: $(BINS)

%.o: %.cpp %.h
	$(CC) $(CFLAGS) -c  $< -o $@

$(BINS): src/main.cpp $(OBJS)
	$(CC) $(CFLAGS)  $^ -o $@

do:
	$(CHECKCC) $(CHECKFLAGS) ./main