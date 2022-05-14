#include "random.h"

int getIntRandom(int lower, int upper){
	return (rand() % (upper - lower + 1)) + lower;
}

double getDoubleRandom(){
	return ((double)random()/RAND_MAX);
}