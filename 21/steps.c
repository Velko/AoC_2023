#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#define     MULTIPLIER  9

#define INPUT_SIZE     140


#define BUFFER_SIZE (INPUT_SIZE * MULTIPLIER)
#define CENTER_OFFSET       (MULTIPLIER / 2)

char orig_garden[INPUT_SIZE][INPUT_SIZE];
int orig_nrows, orig_ncols;

int start_row, start_col;

#define EMPTY_MARKER    '.'
#define REACH_MARKER    'O'
#define ROCK_MARKER     '#'

char garden[BUFFER_SIZE][BUFFER_SIZE];
int visited[BUFFER_SIZE][BUFFER_SIZE];
int nrows, ncols;

static void mark_around(int row, int col, int step);
static void mark_steps(int step);
static int count_reached();

int main(void)
{
    FILE *input = fopen("input.txt", "r");

    orig_nrows = orig_ncols = 0;
    for (;;)
    {
        if (fgets(orig_garden[orig_nrows], INPUT_SIZE, input) == NULL) break;

        if (orig_ncols == 0)
            orig_ncols = strlen(orig_garden[orig_nrows]) - 1;

        char *p = strchr(orig_garden[orig_nrows], 'S');
        if (p)
        {
            start_row = orig_nrows;
            start_col = p - orig_garden[orig_nrows];
        }

        orig_garden[orig_nrows][orig_ncols] = 0; // chomp \n

        ++orig_nrows;
    }
    fclose(input);

    printf("%dx%d\n", orig_nrows, orig_ncols);

    nrows = orig_nrows * MULTIPLIER;
    ncols = orig_ncols * MULTIPLIER;


    start_row += CENTER_OFFSET * orig_nrows;
    start_col += CENTER_OFFSET * orig_ncols;

    for (int r = 0; r < orig_nrows; ++r)
    {
        for (int m = 0; m < MULTIPLIER; ++m)
            memcpy(&garden[r][m * orig_ncols], orig_garden[r], orig_ncols + 1);
    }

    for (int m = 1; m < MULTIPLIER; ++m)
    {
        memcpy(&garden[m * orig_nrows], garden, BUFFER_SIZE * orig_nrows);
    }

    // for (int r = 0; r < nrows; ++r) printf("%s\n", garden[r]);
    // return 0;


    memset(visited, 0, sizeof(visited));

    mark_around(start_row, start_col, 1);

    for (int step = 1; step < 50; ++step)
    {
        mark_steps(step);
        int reached = count_reached();
        printf("Steps: %d Reached: %d\n", step, reached);
    };


    for (int r = 0; r < nrows; ++r) printf("%s\n", garden[r]);

    int total = count_reached();
    // result p1: 3853
    printf("Result: %d\n", total);

    return 0;
}


static void mark_steps(int step)
{
    int copy_visited[BUFFER_SIZE][BUFFER_SIZE];

    memcpy(copy_visited, visited, sizeof(visited));

    for (int r = 0; r < nrows; ++r)
    {
        for (int c = 0; c < ncols; ++c)
        {
            if (copy_visited[r][c] == step)
                garden[r][c] = EMPTY_MARKER;
        }
    }

    for (int r = 0; r < nrows; ++r)
    {
        for (int c = 0; c < ncols; ++c)
        {
            if (copy_visited[r][c] == step)
                mark_around(r, c, step + 1);
        }
    }
}

static void mark_around(int row, int col, int step)
{
    if (row > 0 && garden[row - 1][col] != ROCK_MARKER)
    {
        garden[row - 1][col] = REACH_MARKER;
        visited[row - 1][col] = step;
    }

    if (col > 0 && garden[row][col - 1] != ROCK_MARKER)
    {
        garden[row][col - 1] = REACH_MARKER;
        visited[row][col - 1] = step;
    }

    if (garden[row + 1][col] != ROCK_MARKER)
    {
        garden[row + 1][col] = REACH_MARKER;
        visited[row + 1][col] = step;
    }

    if (garden[row][col + 1] != ROCK_MARKER)
    {
        garden[row][col + 1] = REACH_MARKER;
        visited[row][col + 1] = step;
    }
}

static int count_reached()
{
    int num_reached = 0;
    for (int r = 0; r < nrows; ++r)
    {
        for (int c = 0; c < ncols; ++c)
        {
            if (garden[r][c] == REACH_MARKER)
                ++num_reached;
        }
    }

    return num_reached;
}
