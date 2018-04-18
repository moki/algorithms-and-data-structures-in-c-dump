#include <stdint.h>
#include <limits.h>
#include <stdio.h>

void decasbinprint(unsigned x) {
  int bits = sizeof(unsigned) * CHAR_BIT;
  unsigned i;

  for (i = 1 << (bits - 1); i > 0; i /= 2) {
    printf("%d", (x & i) ? 1 : 0);
  }

  putchar('\n');
}

void displayBinArray(int *xs, int s) {
  int i = 0;
  printf("\n");

  while (i < s) {
    decasbinprint(*(xs + i));
    i++;
  }

  printf("\n");
}

// i - 1
// lsd bit to msd
int isithbitset_lsd(int n, int i) {
  return (n & (1 << (i - 1))) != 0;
}