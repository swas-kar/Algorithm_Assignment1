#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

void swap(int *a, int *b)
{
    int c = *a;
    *a = *b;
    *b = c;
}

// Min function
int min(int a, int b)
{
    return (a < b) ? a : b;
}

// Sort function using bubble sort algorithm
void sort(int arr[], int n)
{
    int i, j, temp;
    for (i = 0; i < n-1; i++)
    {
        for (j = 0; j < n-i-1; j++)
        {
            if (arr[j] > arr[j+1])
            {
                temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}

int medianIndex(int arr[], int start, int final)
{
    int n = final - start + 1;
    int medians[(n + 4) / 5];
    int median_index = 0;

    for (int i = start; i <= final; i += 5)
    {
        int group_start = i;
        int group_final = min(final, group_start + 4);
        int group_size = group_final - group_start + 1;
        int* group = &arr[group_start];
        sort(group, group_size);

        int median = group[group_size / 2];
        medians[median_index++] = median;
    }

    if (median_index == 1)
    {
        return start;
    }
    else
    {
        return medianIndex(medians, 0, median_index - 1);
    }
}


int partition(int arr[], int start, int final, long *comparisons)
{
    int n = final - start + 1;

    // Divide the array into groups of 5
    int num_groups = (n + 4) / 5;
    int medians[num_groups];
    for (int i = 0; i < num_groups; i++)
    {
        int group_start = start + i * 5;
        int group_final = min(final, group_start + 4);
        int median_index = medianIndex(arr, group_start, group_final);
        medians[i] = arr[median_index];
    }

    // Recursively compute median of medians
    int median_of_medians;
    if (num_groups <= 5)
    {
        median_of_medians = medianIndex(medians, 0, num_groups - 1);
    }
    else
    {
        median_of_medians = partition(medians, 0, num_groups - 1, comparisons);
    }

    // Partition using median of medians as pivot
    int pivot_value = medians[median_of_medians];
    int pivot_index = start + (median_of_medians * 5);

    int i = start;
    int j = final;

    while(true)
    {
        while (arr[i] < pivot_value)
            i++;

        while (pivot_value < arr[j])
            j--;

        if (arr[i] == arr[j])
        {
            if (i == j)
                return i;
            else
                j--;
        }
        else if (i < j)
        {
            swap(&arr[i], &arr[j]);
            (*comparisons)++;
        }
        else
            return i;
    }
}

void quickSortMedianOfMedians(int *arr, int start, int final, long *comparisons)
{
    if (start < final)
    {
        int pivot_index = partition(arr, start, final, comparisons);
        quickSortMedianOfMedians(arr, start, pivot_index, comparisons);
        quickSortMedianOfMedians(arr, pivot_index + 1, final, comparisons);
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


    FILE* fp2 = fopen("normal_MoM_observation.csv", "w");

    int max_power = 13;
    int num_of_it = 10;

    if (fp2) {
        fprintf(fp2, "Input size,Best,Worst\n");
        // Start with 2^0 = 1 input and double size each time
        int inputSize = 8;
        for(int p = 0; p < max_power; p++){

            float best_comparison = __INT_MAX__;
            float worst_comparison = 0;

            int input[inputSize];
            text_to_arr(input, inputSize);

            for(int k = 0; k < num_of_it; k++){

                float time = 0;
                long comparisons = 0;

                float start = clock();
                // Sort input data using Quick Sort
                quickSortMedianOfMedians(input, 0, inputSize - 1, &comparisons);
                float end = clock();

                time = (end - start) * 1000 / CLOCKS_PER_SEC;

                if (time > worst_comparison)
                    worst_comparison = time;
                
                if(time < best_comparison)
                    best_comparison = time;
                
                if (is_array_sorted(input, sizeof(input) / sizeof(int)) == 1)
                    printf("Num = %d Run %d: %.4f time, %ld comparisons.\n", inputSize, k + 1, time, comparisons);
                else
                {
                    printf("Array Not Sorted\n");
                    break;
                }

                for (int i = inputSize-1; i >= 0; --i){
                    //generate a random number [0, n-1]
                    int j = rand() % (i+1);

                    //swap the last element with element at random index
                    int temp = input[i];
                    input[i] = input[j];
                    input[j] = temp;
                }
            }

            fprintf(fp2, "%d,%.5f,%.5f\n",inputSize, best_comparison, worst_comparison);

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