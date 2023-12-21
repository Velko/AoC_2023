#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#define     MULTIPLIER  19

#define INPUT_SIZE     140

#define TARGET_STEPS        26501365

#define DBLOCK_SIZE          262

#define FULL_DBLOCKS         4

#define STEP_REMINDER       (TARGET_STEPS % DBLOCK_SIZE)

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
static int count_reached_cell(int cell_y, int cell_x);

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

    printf("%dx%d  *%d\n", orig_nrows, orig_ncols, MULTIPLIER);

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

    printf("Target steps: %d\n", (FULL_DBLOCKS * DBLOCK_SIZE + STEP_REMINDER));

    for (int step = 1; step < (FULL_DBLOCKS * DBLOCK_SIZE + STEP_REMINDER); ++step)
    {
        mark_steps(step);
    };


    //for (int r = 0; r < nrows; ++r) printf("%s\n", garden[r]);

    for (int cy = 0; cy < MULTIPLIER; ++cy)
    {
        for (int cx = 0; cx < MULTIPLIER; ++cx)
        {
            int creach = count_reached_cell(cy, cx);
            printf("%4d ", creach);
        }
        printf("\n");
    }


    int total = count_reached();
    // result p1: 3853
    printf("Result: %d\n", total);

    return 0;
}


int copy_visited[BUFFER_SIZE][BUFFER_SIZE];

static void mark_steps(int step)
{
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


static int count_reached_cell(int cell_y, int cell_x)
{
    int num_reached = 0;
    int offy = cell_y * orig_nrows;
    int offx = cell_x * orig_ncols;

    for (int r = 0; r < orig_nrows; ++r)
    {
        for (int c = 0; c < orig_ncols; ++c)
        {
            if (garden[r + offy][c + offx] == REACH_MARKER)
                ++num_reached;
        }
    }

    return num_reached;
}
