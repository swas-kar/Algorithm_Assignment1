#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void merge(double array[], int left, int middle, int right, int *comparisonCount) {
    int i, j, k;
    int leftSize = middle - left + 1;
    int rightSize = right - middle;

    double leftSubarray[leftSize], rightSubarray[rightSize];

    for (i = 0; i < leftSize; i++)
        leftSubarray[i] = array[left + i];

    for (j = 0; j < rightSize; j++)
        rightSubarray[j] = array[middle + 1 + j];

    i = 0;
    j = 0;
    k = left;

    while (i < leftSize && j < rightSize) {
        if (leftSubarray[i] <= rightSubarray[j]) {
            array[k] = leftSubarray[i];
            i++;
        } else {
            array[k] = rightSubarray[j];
            j++;
        }
        (*comparisonCount)++;
        k++;
    }

    while (i < leftSize) {
        array[k] = leftSubarray[i];
        i++;
        k++;
        (*comparisonCount)++;
    }

    while (j < rightSize) {
        array[k] = rightSubarray[j];
        j++;
        k++;
        (*comparisonCount)++;
    }
}

void mergeSort(double array[], int left, int right, int *comparisonCount) {
    if (left < right) {
        int middle = left + (right - left) / 2;

        mergeSort(array, left, middle, comparisonCount);
        mergeSort(array, middle + 1, right, comparisonCount);

        merge(array, left, middle, right, comparisonCount);
    }
}

int isSorted(double *array, int size) {
    for (int i = 1; i < size; i++) {
        if (array[i] < array[i - 1]) {
            return 0; // Not sorted
        }
    }
    return 1; // Sorted
}

void fillArrayFromCSV(double *array, int size) {
    FILE *csvFile = fopen("./UniformDst.csv", "r");

    double temp = 0.0;
    fscanf(csvFile, "%lf\n", &temp);

    for (int i = 0; i < size; i++) {
        int skip = rand() % 5;

        for (int j = 0; j < skip; j++)
            fscanf(csvFile, "%lf\n", &temp);

        fscanf(csvFile, "%lf\n", &temp);

        array[i] = temp;
    }

    fclose(csvFile);
}

int main() {
    int initialSize = 2;
    int maxPower = 16;
    int iterations = 20;

    FILE *outputFile = fopen("Uniform_comparisons.csv", "w");

    for (int currentPower = 0; currentPower < maxPower; currentPower++) {
        long totalComparisonCount = 0;
        float totalTime = 0;

        int arraySize = initialSize * (1 << currentPower);

        for (int iteration = 0; iteration < iterations; iteration++) {
            double *inputArray = (double *)malloc(arraySize * sizeof(double));
            int comparisonCount = 0;
            fillArrayFromCSV(inputArray, arraySize);

            float startTime = clock();
            mergeSort(inputArray, 0, arraySize - 1, &comparisonCount);
            float endTime = clock();

            totalComparisonCount += comparisonCount;

            float timeTaken = (endTime - startTime) * 1000 / CLOCKS_PER_SEC;
            totalTime += timeTaken;

            if (!isSorted(inputArray, arraySize)) {
                printf("\nArray Not Sorted");
                break;
            }

            free(inputArray);
        }

        float averageTime = totalTime / iterations;
        long averageComparisonCount = totalComparisonCount / iterations;

        fprintf(outputFile, "%d, %0.4f, %ld\n", arraySize, averageTime, averageComparisonCount);
    }

    fclose(outputFile);
    return 0;
}
