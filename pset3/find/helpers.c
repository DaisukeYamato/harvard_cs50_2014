/**
 * helpers.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Helper functions for Problem Set 3.
 */
       
#include <cs50.h>
#include <stdio.h>
#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 * binary search
 */
bool search(int value, int values[], int n) {
  // binary search
  // first, implement sorting
  sort(values, n);
  
  /*for (int i=0; i<n; i++){
    printf("values[%d]: %d\n", i, values[i]);
    }*/
  
  // binary search implementing
  if (n==1){
    if (value==values[0]){
      return true;
    }
    else {
      return false;
    }
  }
  else {
    int even = (n+1) % 2;
    int middle = n/2;
    if (value == values[middle]) { 
      return true; 
    } 
    else if (value < values[middle]){ 
      return search(value, &values[0], middle); 
    }
    else if (value > values[middle]){ 
      return search(value, &values[middle+1], middle - even);
    }
  }

  /* // (old) linear seach
  // implement a searching algorithm
  for (int i=0; i<n; i++){
    if (values[i] == value){
      return true;
    }
  }
  // not found in the list...
  */
  return false;
}

/**
 * Sorts array of n values.
 * (Using bubble sort algorithm)
 */
void sort(int values[], int n) {
  // implement an O(n^2) sorting algorithm
  // bubble sort
  int n_swap = 0;
  int tmp = 0;
  while (true){
    n_swap = 0;
    for (int i=0; i<(n-1); i++){
      if (values[i] > values[i+1]){
	tmp = values[i];
	values[i] = values[i+1];
	values[i+1] = tmp;
	n_swap++;
      }
    }
    if (n_swap==0){
      break;
    }
  }
  return;
}
