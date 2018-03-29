#include <stdio.h>

void SelectionSort(int *a, int size) {
  int i = 0;
  while (i < size) {
    int min = i;
    int j = i + 1;
    
    while (j < size) {
      min = *(a + j) < *(a + min) ? j : min;
      j++;
    }
    
    int t = *(a + i);
    *(a + i) = *(a + min);
    *(a + min) = t;
    i++;
  }
}