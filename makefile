CC = g++
CFLAGS = -g -Wall -O3
BINS = main
SRC_DIRS := src/lib
OUT = test.svg placement.txt cost.out cost.svg

CHECKCC = valgrind
CHECKFLAGS = --leak-check=full -s --show-leak-kinds=all --track-origins=yes 

SRCS := $(wildcard $(SRC_DIRS:=/*.cpp))
OBJS := $(SRCS:.cpp=.o)

all: $(BINS)

%.o: %.cpp %.h
	$(CC) $(CFLAGS) -c  $< -o $@

$(BINS): src/main.cpp $(OBJS)
	$(CC) $(CFLAGS)  $^ -o $@

do1:
	time ./main Benchmark/case1.txt case1.out
	python3 visualize_placement.py
	python3 visualize_cost.py

do2:
	time ./main Benchmark/case2.txt
	python3 visualize_placement.py
	python3 visualize_cost.py

do3:
	time ./main Benchmark/case3.txt
	python3 visualize_placement.py
	python3 visualize_cost.py

do4:
	time ./main Benchmark/case4.txt
	python3 visualize_placement.py
	python3 visualize_cost.py

debug:
	$(CHECKCC) $(CHECKFLAGS) ./main Benchmark/case2.txt
	#python3 visualize_placement.py
	#python3 visualize_cost.py

clean:
	rm -rf src/lib/*.o main $(OUT)