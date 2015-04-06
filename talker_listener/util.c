#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int randomPort()
{
	int number =0;
	int start = 26000;
	srand(time(NULL));
	number = rand() % 500 + start;
    return number;
}