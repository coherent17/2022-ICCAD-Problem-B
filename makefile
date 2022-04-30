CC = g++
CFLAGS = -g -Wall -ggdb3
BINS = main
SRC_DIRS := src/lib
OUT = test.svg placement.txt

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
	python3 visualize_placement.py

clean:
	rm -rf src/lib/*.o main $(OUT)