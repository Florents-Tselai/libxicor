/*
 * $ gcc xicor_example.c -O3 -Wall xicor.c -I. -lm -o xicor_example
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "xicor.h"

int main(int argc, char** argv)
{
    xicor_problem prob;
    xicor_parameter param;
    xicor_score* score;

    double PI = 3.14159265;
    int i;

    printf("libxicor version %s\n", libxicor_version);

    /* set the parameters */
    param.seed = 42;
    param.ties = 0;

    /* build the problem */
    prob.n = 201;
    prob.x = (double*)malloc(prob.n * sizeof(double));
    prob.y = (double*)malloc(prob.n * sizeof(double));
    for (i = 0; i < prob.n; i++)
    {
        /* build x = [0, 0.005, ..., 1] */
        prob.x[i] = (double)i / (double)(prob.n - 1);

        /* build y = sin(10 * pi * x) + x */
        prob.y[i] = sin(10 * PI * prob.x[i]) + prob.x[i];
    }

    /* compute score */
    clock_t start = clock();
    score = xicor_compute_score(&prob, &param);
    clock_t end = clock();
    printf("Elapsed time: %.6f seconds\n\n",
           (double)(end - start) / CLOCKS_PER_SEC);

    if (score == NULL)
    {
        printf("ERROR: xicor_compute_score()\n");
        return 1;
    }

    printf("XICOR: %.10lf\n", score->score);

    /* free prob */
    free(prob.x);
    free(prob.y);

    return 0;
}
