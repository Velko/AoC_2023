#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#define BUFFER_SIZE     150

char maze[BUFFER_SIZE][BUFFER_SIZE];
int nrows, ncols;

int main(void)
{
    FILE *input = fopen("input.txt", "r");

    nrows = ncols = 0;
    for (;;)
    {
        if (fgets(maze[nrows], BUFFER_SIZE, input) == NULL) break;
        if (ncols == 0)
            ncols = strlen(maze[nrows]) - 1;
        maze[nrows][ncols] = 0;
        ++nrows;
    }
    fclose(input);


    for (int r = 0; r < nrows; ++r)
        printf("%s\n", maze[r]);

    return 0;
}
