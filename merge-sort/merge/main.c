#include <stdlib.h>
#include "merge.h"

#define SIZE_A (5)
#define SIZE_B (8)
#define SIZE_D (13)

int main(int argc, char **argv) {
  int a[SIZE_A] = {5, 19, 35, 55, 80};
  int b[SIZE_B] = {7, 15, 25, 40, 60, 77, 85, 100};
  int c[SIZE_A + SIZE_B];
  int d[SIZE_D] = {5, 19, 35, 55, 80, 87, 95, 25, 40, 60, 77, 85, 100};

  twowayMerge(c, a, SIZE_A, b, SIZE_B);

  abstractInplaceMerge(d, 0, (int) (SIZE_D - 1) / 2, (int) SIZE_D - 1);
}