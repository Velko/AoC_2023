#include <stdio.h>


#define BUFFER_SIZE     120

char platform[BUFFER_SIZE][BUFFER_SIZE];

static void roll_all(int nrows);
static void roll_single(int row, int col);

int main(void)
{
    FILE *input = fopen("input.txt", "r");
    int nrows = 0;
    for (;;++nrows)
    {
        if (fgets(platform[nrows], BUFFER_SIZE, input) == NULL) break;
    }
    fclose(input);


    roll_all(nrows);


    for (int row = 0; row < nrows; ++row)
        printf("%s", platform[row]);

    int total = 0;
    printf("Result: %d\n", nrows);

    return 0;
}


static void roll_all(int nrows)
{
    for (int r = 1; r < nrows; ++r)  // no point trying to roll row 0
    {
        for (int c = 0; platform[r][c]; ++c)
        {
            if (platform[r][c] == 'O')
                roll_single(r, c);
        }
    }
}

static void roll_single(int row, int col)
{
    while (row > 0 && platform[row-1][col] == '.')
    {
        platform[row-1][col] = 'O';
        platform[row][col] = '.';
        --row;
    }
}