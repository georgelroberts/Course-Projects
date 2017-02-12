/**
 * helpers.h
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Helper functions for Problem Set 3.
 */
#include <stdbool.h>

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n);

void swap(int *a, int *b);
/**
 * Sorts array of n values.
 */
void insertionSort(int *values, int n);

void selectionSort(int *values, int n);

void bubbleSort(int *values, int n);
