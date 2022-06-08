CC = g++
CFLAGS = -g -Wall -O3
BINS = main
SRC_DIRS := src/lib
OUT = test.svg placement.txt *.out cost.svg

CHECKCC = valgrind
CHECKFLAGS = --leak-check=full -s --show-leak-kinds=all --track-origins=yes 

SRCS := $(wildcard $(SRC_DIRS:=/*.cpp))
OBJS := $(SRCS:.cpp=.o)

all: $(BINS)

%.o: %.cpp %.h
	$(CC) $(CFLAGS) -c  $< -o $@

$(BINS): src/main.cpp $(OBJS)
	$(CC) $(CFLAGS) -fopenmp  $^ -o $@ -lm

do1:
	time ./main Benchmark/case1.txt case1.out
	./Problem\ B_evaluator_0525 Benchmark/case1.txt case1.out
	python3 visualize_placement.py
	#python3 visualize_cost.py

do2:
	time ./main Benchmark/case2.txt case2.out
	./Problem\ B_evaluator_0525 Benchmark/case2.txt case2.out
	python3 visualize_placement.py
	#python3 visualize_cost.py

do3:
	time ./main Benchmark/case3.txt case3.out
	./Problem\ B_evaluator_0525 Benchmark/case3.txt case3.out
	python3 visualize_placement.py
	#python3 visualize_cost.py

do4:
	time ./main Benchmark/case4.txt case4.out
	./Problem\ B_evaluator_0525 Benchmark/case4.txt case4.out
	#python3 visualize_placement.py
	#python3 visualize_cost.py

debug:
	$(CHECKCC) $(CHECKFLAGS) ./main Benchmark/case2.txt case2.out
	#python3 visualize_placement.py
	#python3 visualize_cost.py

clean:
	rm -rf src/lib/*.o main $(OUT)