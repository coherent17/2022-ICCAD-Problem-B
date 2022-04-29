CC = g++
CFLAGS = -g -Wall -ggdb3
BINS = main
SRC_DIRS := src/lib

CHECKCC = valgrind
CHECKFLAGS = --leak-check=full -s --show-leak-kinds=all --track-origins=yes 

SRCS := $(wildcard $(SRC_DIRS:=/*.cpp))
OBJS := $(SRCS:.cpp=.o)

all: $(BINS)

%.o: %.cpp %.h
	$(CC) $(CFLAGS) -c  $< -o $@

$(BINS): src/main.cpp $(OBJS)
	$(CC) $(CFLAGS)  $^ -o $@

do:
	$(CHECKCC) $(CHECKFLAGS) ./main Benchmark/case1.txt

clean:
	rm -rf src/lib/*.o main