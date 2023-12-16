#include <stdio.h>
#include <string.h>


#define BUFFER_SIZE     120

char grid[BUFFER_SIZE][BUFFER_SIZE];
int nrows;
int ncols;


int main(void)
{
    FILE *input = fopen("input.txt", "r");

    nrows = 0;
    for (;;++nrows)
    {
        if (fgets(grid[nrows], BUFFER_SIZE, input) == NULL) break; 
        if (ncols == 0)
            ncols = strlen(grid[nrows]) - 1;
        grid[nrows][ncols] = 0;
    }

    //for (int r = 0; r < nrows; ++r)
    //    printf("'%s'\n", grid[r]);

    int total = 0;
    printf("Result: %d\n", total);

    fclose(input);
    return 0;
}
