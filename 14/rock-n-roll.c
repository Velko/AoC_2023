#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define BUFFER_SIZE     128
#define NUM_CYCLES      1000000000
#define BITS_PER_BYTE   8
#define PACK_SIZE       (BITS_PER_BYTE * sizeof(unsigned long long))

char platform[BUFFER_SIZE][BUFFER_SIZE];
int nrows;
int ncols;

struct packed_platform
{
    unsigned long long bits[BUFFER_SIZE][BUFFER_SIZE / PACK_SIZE];
};

#define MAX_HISTORY     256  // let's try with something, then will see
struct packed_platform history[MAX_HISTORY];
int nhistory;


static void roll_all_north();
static void roll_all_west();
static void roll_all_south();
static void roll_all_east();
static int count_load();
static void pack_state(struct packed_platform *packed);
static int find_loop();

int main(void)
{
    FILE *input = fopen("input.txt", "r");

    memset(platform, 0, sizeof(platform));

    nrows = 0;
    for (;;++nrows)
    {
        if (fgets(platform[nrows], BUFFER_SIZE, input) == NULL) break;
    }
    fclose(input);

    ncols = strlen(platform[0]) - 1; // discard \0


    pack_state(history); // place initial state
    nhistory = 1;

    int loop_start = 0;

    int cycles;
    for (cycles = 0; loop_start == 0; ++cycles)
    {
        roll_all_north();
        roll_all_west();
        roll_all_south();
        roll_all_east();
        loop_start = find_loop();
    }

    int loop_size = nhistory - loop_start;
    printf("Loop found [%d..%d) size %d\n", loop_start, nhistory, loop_size);

    int loops = (NUM_CYCLES - loop_start - loop_size) / loop_size;
    cycles += loops * loop_size;
    printf("Skipped %d loops, restarting at %d\n", loops, cycles);

    for (; cycles < NUM_CYCLES; ++cycles)
    {
        roll_all_north();
        roll_all_west();
        roll_all_south();
        roll_all_east();
    }

    int load = count_load();

    // result p1: 110779
    // result p2: 86069
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


static int find_loop()
{
    //pack new state
    pack_state(history + nhistory);

    // for (int r = 0; r < BUFFER_SIZE; ++r)
    // {
    //     for (unsigned p = 0; p < BUFFER_SIZE / PACK_SIZE; ++p)
    //         printf("%llx", history[nhistory].bits[r][p]);
    // }
    // printf("\n");

    // check if seen before
    for (int h = 0; h < nhistory; ++h)
    {
        if (memcmp(history + h, history + nhistory, sizeof(struct packed_platform)) == 0)
            return h;
    }

    ++nhistory;
    if (nhistory >= MAX_HISTORY)
    {
        printf("History full\n");
        exit(1);
    }

    return 0;
}


static void pack_state(struct packed_platform *packed)
{
    memset(packed, 0, sizeof(struct packed_platform));

    for (int r = 0; r < nrows; ++r)
    {
        for (int c = 0; c < ncols; ++c)
        {
            if (platform[r][c] == 'O')
            {
                packed->bits[r][c / PACK_SIZE] |= 1ULL << (c % PACK_SIZE);
            }
        }
    }
}