#include <stdio.h>
#include <stdlib.h>

struct student
{
    char name[100]; // No one should have a name longer than this
    int *grades;    // This is the pointer to hold the (integer) grades
    int count;      // The number of grades
};

float average(struct student s)
{
    float total = 0;

    for(int i = 0; i < s.count; i++)
    {
        total += s.grades[i];
    }

    return total / s.count;
}

int main(int argc, char **argv)
{
    int num_students = 0;
    struct student *students;
    
    printf("How many students are there? ");
    scanf("%d", &num_students);

    students = malloc(num_students * sizeof(struct student));

    for(int i = 0; i < num_students; i++)
    {
        printf("Enter the name of student %d: ", i);
        scanf("%s", students[i].name); //might be a &students[i].name

        printf("How many grades does %s have? ", students[i].name);
        scanf("%d", &students[i].count);

        students[i].grades = malloc(students[i].count * sizeof(int));

        for(int j = 0; j < students[i].count; j++)
        {
            printf("Enter the next grade: ");
            scanf("%d", &students[i].grades[j]);
        }

        printf("\n");
    }

    for (int i = 0; i < num_students; i++)
    {
        printf("%s has an average grade of %.2f\n", students[i].name, average(students[i]));
    }
}
