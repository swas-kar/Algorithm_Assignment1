#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAX_SIZE 50000
#define NUM_ITER 10

void insSort(double arr[], int start, int end) {
    for (int i = start; i <= end; i++) {
        double val = arr[i];
        int pos = i - 1;
        while (pos >= start && arr[pos] > val) {
            arr[pos + 1] = arr[pos];
            pos--;
        }
        arr[pos + 1] = val;
    }
}

double getMed(double arr[], int start, int end) {
    insSort(arr, start, end);
    int mid = (start + end) / 2;
    return arr[mid];
}

double mom(double arr[], int size, int div) {
    if (size <= div) return getMed(arr, 0, size - 1);

    int groups = size / div;
    int remain = size % div;
    int nextSize = (remain == 0) ? groups : groups + 1;
    double nextArr[nextSize];

    for (int i = 0; i < nextSize; i++) {
        int s = div * i;
        int e = (i == nextSize - 1) ? size - 1 : div * (i + 1) - 1;
        nextArr[i] = getMed(arr, s, e);
    }

    return mom(nextArr, nextSize, div);
}

void swap(double *a, double *b) {
    double tmp = *a;
    *a = *b;
    *b = tmp;
}

int partition(double arr[], int low, int high) {
    double pivot = arr[high];
    int i = low - 1;

    for (int curr = low; curr <= high; curr++) {
        if (arr[curr] < pivot) {
            i++;
            swap(&arr[i], &arr[curr]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

int findPart(double *arr, int size, int div) {
    double pivotVal = mom(arr, size, div);

    for (int i = 0; i < size; i++) {
        if (arr[i] == pivotVal) {
            swap(&arr[size - 1], &arr[i]);
            return partition(arr, 0, size - 1);
        }
    }
    return -1;
}

void readData(double *arr, int size) {
    FILE *fin = fopen("./NormalDist.txt", "r");
    double temp;

    fscanf(fin, "%*f\n");
    for (int i = 0; i < size; i++) {
        int skip = rand() % 5;
        for (int j = 0; j < skip; j++) {
            fscanf(fin, "%*f\n");
        }
        fscanf(fin, "%lf\n", &temp);
        arr[i] = temp;
    }

    fclose(fin);
}

int main() {
    srand(time(0));

    FILE *fout = fopen("mom_normal_obs.txt", "w");
    if (fout == NULL) {
        printf("Error opening output file.\n");
        return 1;
    }

    fprintf(fout, "Sz, PartSz, Time\n");

    for (int d = 3; d <= 7; d += 2) {
        for (int s = 100; s <= MAX_SIZE; s += 100) {
            double avgTime = 0;

            for (int iter = 0; iter < NUM_ITER; iter++) {
                double arr[s];
                readData(arr, s);

                clock_t start = clock();
                int partInd = findPart(arr, s, d);
                clock_t end = clock();

                double time = ((double)(end - start) / CLOCKS_PER_SEC) * 1000;
                avgTime += time;
            }

            avgTime /= NUM_ITER;

            printf("Sz: %d, PartSz: %d, Time: %.2f\n", s, d, avgTime);
            fprintf(fout, "%d, %d, %.2f\n", s, d, avgTime);
        }
    }

    fclose(fout);
    return 0;
}
