#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "xicor.h"

#define MAX_LINE_LEN 1024 * 1024 * 1024
#define MAX_COLUMNS 100

/* Function to read CSV and dynamically extract columns */
int read_csv_dynamic(const char *filename, double ***columns, char ***col_names, int *n_rows, int *n_cols)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        fprintf(stderr, "Error: Could not open file %s\n", filename);
        return -1;
    }

    char line[MAX_LINE_LEN];
    int line_count = 0;

    /* Read the header */
    if (!fgets(line, MAX_LINE_LEN, file))
    {
        fprintf(stderr, "Error: Failed to read header\n");
        fclose(file);
        return -1;
    }

    /* Parse the header to extract column names */
    char *token;
    int col_count = 0;
    *col_names = (char **)malloc(MAX_COLUMNS * sizeof(char *));
    token = strtok(line, ",\n");
    while (token)
    {
        (*col_names)[col_count] = strdup(token);
        col_count++;
        token = strtok(NULL, ",\n");
    }
    *n_cols = col_count;

    /* Allocate memory for columns */
    *columns = (double **)malloc(col_count * sizeof(double *));
    for (int i = 0; i < col_count; i++)
    {
        (*columns)[i] = (double *)malloc(MAX_LINE_LEN * sizeof(double));
    }

    /* Read the data rows */
    int max_rows = 100; /* Initial allocation for rows */
    *n_rows = 0;

    while (fgets(line, MAX_LINE_LEN, file))
    {
        if (*n_rows >= max_rows)
        {
            max_rows *= 2;
            for (int i = 0; i < col_count; i++)
            {
                (*columns)[i] = (double *)realloc((*columns)[i], max_rows * sizeof(double));
            }
        }

        /* Parse the row */
        token = strtok(line, ",\n");
        int col_index = 0;
        while (token && col_index < col_count)
        {
            (*columns)[col_index][*n_rows] = atof(token);
            token = strtok(NULL, ",\n");
            col_index++;
        }
        (*n_rows)++;
    }

    fclose(file);
    return 0;
}

/* Function to find the column index by name */
int get_column_index(char **col_names, int n_cols, const char *target_name)
{
    for (int i = 0; i < n_cols; i++)
    {
        if (strcasecmp(col_names[i], target_name) == 0)
        {
            return i;
        }
    }
    return -1;
}

/* Function to compute and print XICOR scores */
void compute_xicor(double *a, double *b, int n, const char *name_a, const char *name_b)
{
    xicor_problem prob;
    xicor_parameter param;
    xicor_score *score;

    /* Set up problem */
    prob.n = n;
    prob.x = a;
    prob.y = b;

    /* Set parameters */
    param.seed = 42;
    param.ties = 0;

    /* Compute score */
    clock_t start = clock();
    score = xicor_compute_score(&prob, &param);
    clock_t end = clock();

    if (score == NULL)
    {
        fprintf(stderr, "Error: xicor_compute_score() failed for (%s, %s)\n", name_a, name_b);
        return;
    }

    printf("XICOR(%s, %s): %.10lf (Elapsed time: %.6f seconds)\n",
           name_a, name_b, score->score, (double)(end - start) / CLOCKS_PER_SEC);
}

int main(int argc, char **argv)
{
    if (argc != 4)
    {
        fprintf(stderr, "Usage: %s <csv_file> <column_name(Y)> <n_lines>\n", argv[0]);
        return 1;
    }

    const char *csv_file = argv[1];
    const char *column_name = argv[2];
    int n_lines = atoi(argv[3]);

    if (n_lines <= 0)
    {
        fprintf(stderr, "Error: n_lines must be greater than 0\n");
        return 1;
    }

    double **columns = NULL;
    char **col_names = NULL;
    int n_rows = 0, n_cols = 0;

    /* Read CSV file */
    if (read_csv_dynamic(csv_file, &columns, &col_names, &n_rows, &n_cols) != 0)
    {
        return 1;
    }

    if (n_rows < n_lines)
    {
        fprintf(stderr, "Warning: CSV file has only %d rows, adjusting n_lines to %d\n", n_rows, n_rows);
        n_lines = n_rows;
    }

    /* Find column indices */
    int y_idx = get_column_index(col_names, n_cols, column_name);
    int x_idx = get_column_index(col_names, n_cols, "x");
    int z_idx = get_column_index(col_names, n_cols, "z");

    if (y_idx == -1 || x_idx == -1 || z_idx == -1)
    {
        fprintf(stderr, "Error: Could not find required columns 'x', 'z', or '%s' in CSV\n", column_name);
        goto cleanup;
    }

    /* Compute and print scores */
    printf("Computing XICOR scores for first %d rows of column '%s'...\n\n", n_lines, column_name);
    compute_xicor(columns[x_idx], columns[y_idx], n_lines, "X", column_name);
    compute_xicor(columns[z_idx], columns[y_idx], n_lines, "Z", column_name);

cleanup:
    /* Free memory */
    for (int i = 0; i < n_cols; i++)
    {
        free(columns[i]);
        free(col_names[i]);
    }
    free(columns);
    free(col_names);

    return 0;
}