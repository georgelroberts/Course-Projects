/**
 * find.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Prompts user for as many as MAX values until EOF is reached, 
 * then proceeds to search that "haystack" of values for given needle.
 *
 * Usage: ./find needle
 *
 * where needle is the value to find in a haystack of values
 */
#define _CRT_SECURE_NO_WARNING
#define _XOPEN_SOURCE


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#include "helpers.h"
#include "cs50.h"

// maximum amount of hay
const int MAX = 100;

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: ./find needle\n");
        return -1;
    }

    // remember needle
    int needle = atoi(argv[1]);
	printf("Searching for %d in a randomly generated haystack\n\n",needle);

    // fill haystack
    int size=MAX;
	int* haystack = malloc(sizeof(int)*MAX);
	srand((long int)time(NULL));

	int tempRand;
	// Generate the number of random bits required (with a maximum of 65536 (2^16))
	printf("Haystack: \n");
	for (int i = 0; i < MAX; i++)
	{
		tempRand = (int)(rand()% MAX);
		*(haystack+i)= tempRand;
		printf("%d ", tempRand);
	}

    // sort the haystack
    bubbleSort(haystack, size);  
	printf("\n\nSorted:\n");
	for (int i = 0;i<MAX;i++)
		printf("%d ", *(haystack + i));

    // try to find needle in haystack
    if (search(needle, haystack, size))
    {
        printf("\nFound needle in haystack!\n\n");
        return 0;
    }
    else
    {
        printf("\nDidn't find needle in haystack.\n\n");
        return 1;
    }
}