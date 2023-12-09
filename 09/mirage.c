#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


#define BUFFER_SIZE     256
#define NNUMBERS_MAX     25



static int parse_line(char *line);


int numbers[NNUMBERS_MAX][NNUMBERS_MAX];


int main(void)
{
    char line[BUFFER_SIZE];
    FILE *input = fopen("input.txt", "r");

    int total = 0;

    for (;;)
    {
        if (fgets(line, BUFFER_SIZE, input) == NULL) break; 
        total += parse_line(line);
    }

    printf("Result: %d\n", total);

    fclose(input);
    return 0;
}


static int parse_line(char *line)
{
    char *savep;

    char *token = strtok_r(line, " ", &savep);

    int nvalues = 0;
    while (token)
    {
        numbers[0][nvalues++] = atoi(token);
        printf("%d ", numbers[0][nvalues-1]);
        token = strtok_r(NULL, " ", &savep);
    }

    printf("\n");

    bool all_zeros = false;
    int cnumvalues = nvalues - 1;
    int row;
    for (row = 1; !all_zeros ; ++row)
    {
        all_zeros = true;
        for (int i = 0; i < cnumvalues; ++i)
        {
            numbers[row][i] = numbers[row-1][i+1] - numbers[row-1][i];
            printf("%d ", numbers[row][i]);
            all_zeros &= numbers[row][i] == 0;
        }
        --cnumvalues;
        printf("\n");
    }
    printf("r: %d n:%d\n", row, cnumvalues);


    row -= 2;
    int nextval = 0;
    for (;row >= 0; --row)
    {
        nextval = numbers[row][0] - nextval;
        printf("%d\n", nextval);
    }

    printf("\n");

    return nextval;
}