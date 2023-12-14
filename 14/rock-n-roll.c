#include <stdio.h>
#include <string.h>


#define BUFFER_SIZE     120

char platform[BUFFER_SIZE][BUFFER_SIZE];
int nrows;
int ncols;

static void roll_all_north();
static int count_load();

int main(void)
{
    FILE *input = fopen("input.txt", "r");
    nrows = 0;
    for (;;++nrows)
    {
        if (fgets(platform[nrows], BUFFER_SIZE, input) == NULL) break;
    }
    fclose(input);

    ncols = strlen(platform[0]) - 1; // discard \0

    roll_all_north();


    // for (int row = 0; row < nrows; ++row)
    //     printf("%s", platform[row]);

    int load = count_load();

    // result p1: 110779
    printf("Result: %d\n", load);

    return 0;
}


static void roll_all_north()
{
    for (int r = 1; r < nrows; ++r)  // no point trying to roll row 0
    {
        for (int c = 0; c < ncols; ++c)
        {
            if (platform[r][c] == 'O')
            {
                for (int roll_r = r; roll_r > 0 && platform[roll_r-1][c] == '.'; --roll_r)
                {
                    platform[roll_r-1][c] = 'O';
                    platform[roll_r][c] = '.';
                }
            }
        }
    }
}


static int count_load()
{
    int load = 0;
    for (int row = 0; row < nrows; ++row)
    {
        for (int col = 0; col < ncols; ++col)
        {
            if (platform[row][col] == 'O')
                load += nrows - row;
        }
    }

    return load;
}