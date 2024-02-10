#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#define MAX_LINES 10000000
#define MAX_VALUE 200

int NO_OF_LINES;

// Function to swap two elements
void swap(double *a, double *b) {
    double temp = *a;
    *a = *b;
    *b = temp;
}

// Partition function for quicksort
int partition(double arr[], int low, int high) {
    double pivot = arr[high];  // pivot
    int i = (low - 1);  // Index of smaller element

    for (int j = low; j <= high - 1; j++) {
        // If current element is smaller than the pivot
        if (arr[j] < pivot) {
            i++;    // increment index of smaller element
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

// Function to implement randomized quicksort
void quickSort(double arr[], int low, int high) {
    if (low < high) {
        // Partitioning index
        int pi = partition(arr, low, high);

        // Separately sort elements before partition and after partition
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// Function to normalize the data and write to file
void normalizeAndWriteToFile(double arr[], int size, const char *output_file) {
    FILE *fout = fopen(output_file, "w");
    fprintf(fout, "%d\n", size);

    for (int i = 0; i < size; i++) {
        fprintf(fout, "%0.2f\n", arr[i] / MAX_VALUE);
    }

    fclose(fout);
}

int main() {
    srand(time(NULL)); // Seed for randomization

    // Read normal distribution data
    FILE *fin_normal = fopen("./NormalDist.csv", "r");
    fscanf(fin_normal, "%d\n", &NO_OF_LINES);
    double *normal_data = (double *)malloc(NO_OF_LINES * sizeof(double));

    for (int i = 0; i < NO_OF_LINES; i++) {
        fscanf(fin_normal, "%lf\n", &normal_data[i]);
    }
    fclose(fin_normal);

    // Sort and normalize normal distribution data
    quickSort(normal_data, 0, NO_OF_LINES - 1);
    normalizeAndWriteToFile(normal_data, NO_OF_LINES, "./normalized_normal_distribution.csv");
    free(normal_data);

    // Read uniform distribution data
    FILE *fin_uniform = fopen("./UniformDist.csv", "r");
    fscanf(fin_uniform, "%d\n", &NO_OF_LINES);
    int *uniform_data = (int *)malloc(NO_OF_LINES * sizeof(int));

    for (int i = 0; i < NO_OF_LINES; i++) {
        fscanf(fin_uniform, "%d\n", &uniform_data[i]);
    }
    fclose(fin_uniform);

    // Sort and normalize uniform distribution data
    quickSort(uniform_data, 0, NO_OF_LINES - 1);
    normalizeAndWriteToFile(uniform_data, NO_OF_LINES, "./normalized_uniform_distribution.csv");
    free(uniform_data);

    return 0;
}
