typedef struct dual_pivot dual_pivot;

void exchange(int *, int, int);

int partition(int *, int, int);

void recursive_quicksort(int *, int, int);

void tail_recursive_quicksort(int *, int, int);

void three_way_quicksort(int *, int, int);

dual_pivot dual_pivot_partition(int *, int, int);

void dual_pivot_quicksort(int *, int, int);