#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

// An utility function to swap two int values
void swap(int *a, int *b)
{
    int c = *a;
    *a = *b;
    *b = c;
}

int partition(int arr[], int low, int high, long *count)
{
    int pivot = arr[high];
    int i = low - 1;

    for (int curr = low; curr <= high; curr++)
    {
        if (arr[curr] < pivot)
        {
            i++;
            swap(&arr[i], &arr[curr]);
            (*count)++;
        }
    }
    swap(&arr[i + 1], &arr[high]);
    (*count)++;
    return (i + 1);
}

void quickSort(int *arr, int start, int final, long *comparisons)
{
    if (start < final)
    {
        int pivot_index = partition(arr, start, final, comparisons);
        quickSort(arr, start, pivot_index, comparisons);
        quickSort(arr, pivot_index + 1, final, comparisons);
    }
}

//assuming 1 is true, 0 is false
int is_array_sorted(int *arr, int length)
{
    for (int i = 0; i < length - 1; i++)
        if (arr[i] > arr[i + 1])
            return 0;

    return 1;
}

void text_to_arr(int *arr, int n)
{
    FILE *fin = fopen("NormalDist.csv", "r");

    int temp = 0;
    char temp2[10];
    fscanf(fin, "%s\n", temp2);
    
    for (int i = 0; i < n; i++)
    {
        int skip_line = rand() % 10;

        for (int j = 0; j < skip_line; j++)
            fscanf(fin, "%d\n", &temp);

        fscanf(fin, "%d\n", &temp);
        arr[i] = (int)temp;
    }

    fclose(fin);
}

int main()
{
    srand(time(0));

    FILE* fp2 = fopen("normal_rand_quick.csv", "w");

    int max_power = 17;
    int num_of_it = 100;

    if (fp2) {
        fprintf(fp2, "Input size,Comparisons,Time (ms)\n");
        // Start with 2^0 = 1 input and double size each time
        int inputSize = 2;
        for(int p = 0; p < max_power; p++){
            long total_comparisons = 0;
            float total_time = 0;
            for(int k = 0; k < num_of_it; k++){

                int input[inputSize];

                text_to_arr(input, inputSize);

                long comparisons = 0;
                
                // Record start time
                float start = clock();
                // Sort input data using Quick Sort
                quickSort(input, 0, inputSize - 1, &comparisons);
                // Record end time
                float end = clock();
                // Calculate time taken in milliseconds
                float timeTaken = (end - start) * 1000 / CLOCKS_PER_SEC ;

                total_comparisons += comparisons;
                total_time += timeTaken;
                if (is_array_sorted(input, sizeof(input) / sizeof(int)) == 1)
                    printf("Num = %d Run %d: %d comparison, %0.4f ms\n", inputSize, k + 1, comparisons, timeTaken);
                else
                {
                    printf("Array Not Sorted\n");
                    break;
                }
            }

            long avg_comparisons = total_comparisons / num_of_it;
            float avg_time = total_time / num_of_it;
            
            // Write results to output file
            fprintf(fp2, "%d,%ld,%0.4f\n", inputSize, avg_comparisons, avg_time);

            // Double input size for next iteration
            inputSize *= 2;
        }
        fclose(fp2);
    } else {
        printf("Failed to open output file: %s\n", "failed.csv");
        return 1;
    }

    return 0;
}
