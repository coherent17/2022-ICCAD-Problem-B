CC = g++
CFLAGS = -g -Wall
BINS = main
SRC_DIRS := src/lib
OUTPUT_FILE = Netlist* log.txt


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

clean:
	rm -rf $(OUTPUT_FILE)  src/lib/*.o main