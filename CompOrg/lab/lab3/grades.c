#include <stdio.h>
#include <stdlib.h>

int main(int argc, int **argv)
{
    int *grades;
    int count;
    float total = 0;
    float avg;

    printf("How many grades does the student have? ");
    scanf("%d", &count);

    grades = malloc(count * sizeof(int));


    for(int i = 0; i < count; i++) 
    {
        printf("Enter the next grade: ");
        scanf("%d", &grades[i]);
        total += grades[i];
    }

    avg = total / count;

    printf("The average grade is %.2f\n", avg);
}
    
