#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define ARR_DIM 50
#define BIG_VAL 500000

int main()
{
    int i, j;
    int ***array3d = 0;
    
    array3d = malloc(ARR_DIM * sizeof(int **));
    
    assert(array3d); // Same as saying assert(array3d != 0)
    
    for (i = 0; i < ARR_DIM; i++) {
        array3d[i] = (int **)malloc(ARR_DIM * sizeof(int *));
        assert(array3d[i]); // Same as saying assert(array3d[i] != 0)
        for (j = 0; j < ARR_DIM; j++) {
            array3d[i][j] = (int *)malloc(ARR_DIM * sizeof(int));
            assert(array3d[i][j]); // Same as saying assert(array3d[i][j] != 0)
        }
    }

    // Make sure we can store our value here
    assert(BIG_VAL < INT_MAX);
    array3d[0][0][0] = BIG_VAL;
    
    return 0;
}
