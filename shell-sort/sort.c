void ShellSort(int *a, int size) {
  int h = 1;

  while (h < size / 3) h = 3 * h + 1;

  while (h >= 1) {
    int i = h;

    while (i < size) {
      int j = i;

      while (j >= h && *(a + j) < *(a + j - h)) {
        int t = *(a + j);
        *(a + j) = *(a + j - h);
        *(a + j - h) = t;

        j -= h;
      }

      i++;
    }

    h /= 3;
  }
}