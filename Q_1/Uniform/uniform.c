#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int uniformGenerator(int lo, int hi) {
    int range = hi - lo + 1;
    double frac = rand() / (1.0 + RAND_MAX);
    return (int)(frac * range + lo);
}

int main() {
    int n, low, high;
    char filename[40] = "UniformDist.csv";

    srand(time(0));

    low = 1;
    high = 200;
    n = 100000;

    int *arr = (int *)malloc(n * sizeof(int));
    FILE *fp;

    fp = fopen(filename, "w");

    if (fp == NULL) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        return 1;
    }

    for (int i = 0; i < n; i++) {
        arr[i] = uniformGenerator(low, high);
        fprintf(fp, "%d\n", arr[i]);
    }

    fclose(fp);
    free(arr);

    return 0;
}
