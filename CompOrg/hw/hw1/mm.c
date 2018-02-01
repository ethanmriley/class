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
    for(int i = 0; i < rows; i++)
    {
        free(matrix[i]);
    }
    free(matrix);
}

void mm_read( int **matrix, int rows, int columns )
{
    printf("Please enter the values for the %d x %d matrix:\n", rows, columns);
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < columns; j++)
        {
            scanf("%d", &matrix[i][j]);
        }
    }

}

void mm_print( int **matrix, int rows, int columns )
{
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < columns; j++)
        {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void mm_mult( int **m1, int m1_rows, int m1_cols,
              int **m2, int m2_rows, int m2_cols,
              int **results)
{
    for(unsigned int i = 0; i < m1_rows; i++) //TODO might be cols here
    {
        for(unsigned int j = 0; j < m1_rows; j++)
        {
            for(unsigned int k = 0; k < m2_rows; k++)
            {
                results[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }
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

    printf("How many rows are in the first matrix?\n");
    scanf("%d", &m1_rows);
    printf("How many columns are in the first matrix?\n");
    scanf("%d", &m1_columns);

    printf("How many rows are in the second matrix?\n");
    scanf("%d", &m2_rows);
    printf("How many columns are in the second matrix?\n");
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
