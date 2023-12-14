#include <stdio.h>
#include <string.h>


#define BUFFER_SIZE     120
#define NUM_CYCLES      1000000000

char platform[BUFFER_SIZE][BUFFER_SIZE];
int nrows;
int ncols;

static void roll_all_north();
static void roll_all_west();
static void roll_all_south();
static void roll_all_east();
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

    for (int cycles = 0; cycles < NUM_CYCLES; ++cycles)
    {
        roll_all_north();
        roll_all_west();
        roll_all_south();
        roll_all_east();

        if (cycles % (NUM_CYCLES / 1000) == 0)
            printf("%g\n", cycles * 100.0 / NUM_CYCLES);
    }

    for (int row = 0; row < nrows; ++row)
         printf("%s", platform[row]);

    int load = count_load();

    // result p1: 110779
    printf("Result: %d\n", load);

    return 0;
}


static void roll_all_north()
{
    for (int c = 0; c < ncols; ++c)
    {
        for (int r = 1; r < nrows; ++r)  // no point trying to roll row 0
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

static void roll_all_west()
{
    for (int r = 0; r < nrows; ++r)
    {
        for (int c = 1; c < ncols; ++c)  // no point trying to roll col 0
        {
            if (platform[r][c] == 'O')
            {
                for (int roll_c = c; roll_c > 0 && platform[r][roll_c-1] == '.'; --roll_c)
                {
                    platform[r][roll_c-1] = 'O';
                    platform[r][roll_c] = '.';
                }
            }
        }
    }
}

static void roll_all_south()
{
    for (int c = 0; c < ncols; ++c)
    {
        for (int r = nrows - 2; r >= 0; --r)  // no point trying to roll last row
        {
            if (platform[r][c] == 'O')
            {
                for (int roll_r = r; roll_r < nrows && platform[roll_r+1][c] == '.'; ++roll_r)
                {
                    platform[roll_r+1][c] = 'O';
                    platform[roll_r][c] = '.';
                }
            }
        }
    }
}


static void roll_all_east()
{
    for (int r = 0; r < nrows; ++r)
    {
        for (int c = ncols - 2; c >= 0; --c)  // no point trying to roll last col
        {
            if (platform[r][c] == 'O')
            {
                for (int roll_c = c; roll_c < ncols && platform[r][roll_c+1] == '.'; ++roll_c)
                {
                    platform[r][roll_c+1] = 'O';
                    platform[r][roll_c] = '.';
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