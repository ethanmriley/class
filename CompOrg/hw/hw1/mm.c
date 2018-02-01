/*****************************************************************************/
/* Assignment 1: 2-D/ Matrix Multiplication **********************************/
/* Name:     Ethan Riley     *************************************************/
/*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

int **mm_alloc( int rows, int columns )
{
    /*
     * hints: allocate an array of pointers to ints, then allocate
     * an row/array of ints and assign each int pointer that row
     * see slide 62
     */

    int **matrix = malloc(sizeof(int *)*rows);
    for(int i = 0; i < rows; i++)
    {
        int *row = malloc(sizeof(int *)*columns);
        matrix[i] = row;
    }

    return matrix;
}

void mm_free( int **matrix, int rows, int columns )
{
}

void mm_read( int **matrix, int rows, int columns )
{
    char buf[50];
    printf("Please enter the values for the %d x %d matrix:\n", rows, columns);
    fgets(buf, sizeof(buf), stdin);
    for(int i = 0; i < rows; i++)
    {
        fgets(buf, sizeof(buf), stdin);
        int j, value_int = 0;
        char * value_str;
        value_str = strtok(buf, " ");
        //TODO figure out what's going on here 
        while(j < columns && value_str != NULL)
        {
            //printf("%s", value_str);
            value_int = atoi(value_str);
            printf("value_int: %d  i: %d  j: %d\n", value_int, i, j);
            matrix[i][j] = value_int;
            value_str = strtok(NULL, " ");
            j++;
        }
    }
}

void mm_print( int **matrix, int rows, int columns )
{
    //printf("\n");
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < columns; j++)
        {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
    //printf("\n");
}

void mm_mult( int **m1, int m1_rows, int m1_cols,
              int **m2, int m2_rows, int m2_cols,
              int **results)
{
}

int main()
{
    /*
     * You must keep this function AS IS.
     * You are not allowed to modify it!
     */
    int **matrix1 = NULL;
    int **matrix2 = NULL;
    int **results_matrix = NULL;

    int m1_rows, m1_columns;
    int m2_rows, m2_columns;

    printf("How many rows are in the first matrix? ");
    scanf("%d", &m1_rows);
    printf("How many columns are in the first matrix? ");
    scanf("%d", &m1_columns);

    printf("How many rows are in the second matrix? ");
    scanf("%d", &m2_rows);
    printf("How many columns are in the second matrix? ");
    scanf("%d", &m2_columns);

    printf("\n");

    if (m1_columns != m2_rows) {
        printf("Invalid matrix multiplication!\n");
        exit(-1);
    }

    matrix1 = mm_alloc(m1_rows, m1_columns);
    matrix2 = mm_alloc(m2_rows, m2_columns);

    results_matrix = mm_alloc(m1_rows, m2_columns);

    mm_read( matrix1, m1_rows, m1_columns );
    mm_read( matrix2, m2_rows, m2_columns );

    printf("\n");

    mm_mult(matrix1, m1_rows, m1_columns, matrix2, m2_rows, m2_columns, results_matrix);

    mm_print( matrix1, m1_rows, m1_columns );
    printf("\n");
    printf("multiplied by\n\n");
    mm_print( matrix2, m2_rows, m2_columns );
    printf("\n");
    printf("is: \n\n");
    mm_print( results_matrix, m1_rows, m2_columns );
    mm_free( matrix1, m1_rows, m1_columns );
    mm_free( matrix2, m2_rows, m2_columns );
    mm_free( results_matrix, m1_rows, m2_columns );

    return 0;
}