#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NMAX 10000000

double *generate(int n, double miu, double sigma) {
    int i, j;
    double *values = (double *)calloc(n, sizeof(double));

    if (values) {
        // Seed the random number generator with the current time
        srand((unsigned int)time(NULL));

        for (i = 0; i < n; i++) {
            double sum = 0.0;
            // Generate 12 random numbers and sum them up
            for (j = 0; j < 12; j++) {
                sum += (rand() / (double)RAND_MAX);
            }
            // Transform the sum to have mean miu and standard deviation sigma
            values[i] = miu + sigma * (sum - 6.0);
        }
    }
    return values;
}

int main(void) {
    double *seq, miu, sigma;
    long int n = 1000000;
    char filename[30] = "NormalDist.csv";
    miu = 100.0;
    sigma = 10.0;
    seq = generate(n, miu, sigma);
    FILE *fp;
    fp = fopen(filename, "w");
    fprintf(fp, "%ld\n", n);
    for (int i = 0; i < n; i++) {
        fprintf(fp, "%lf\n", seq[i]);
    }
    fclose(fp);
    free(seq); // Don't forget to free the allocated memory
    return 0;
}
