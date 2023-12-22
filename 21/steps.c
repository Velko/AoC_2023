#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

//#define DEBUG_PRINT

#define     MULTIPLIER  5

#define INPUT_SIZE     140

#define TARGET_STEPS_P1           64
#define TARGET_STEPS_P2     26501365

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

    memset(visited, 0, sizeof(visited));

    mark_around(start_row, start_col, 1);

    int step;
    for (step = 1; step < TARGET_STEPS_P1; ++step)
    {
        mark_steps(step);
    }

    int result1 = count_reached_cell(CENTER_OFFSET, CENTER_OFFSET);
    // result p1: 3853
    printf("Result p1: %d\n", result1);

    #ifdef DEBUG_PRINT
        for (int r = orig_nrows * CENTER_OFFSET; r < (orig_nrows * (CENTER_OFFSET + 1)); ++r)
        {
            char line[INPUT_SIZE];
            strncpy(line, garden[r] + orig_ncols * CENTER_OFFSET, orig_ncols);
            line[orig_ncols] = 0;
            printf("%s\n", line);
        }
    #endif

    // minimum distance has to be 2x the size of original garden
    // + whatever is the reminder when target step count is divided
    // by that
    assert(orig_ncols == orig_nrows);
    int double_block = orig_ncols * 2;
    int step_reminder = TARGET_STEPS_P2 % double_block;
    long target_dblocks = TARGET_STEPS_P2 / double_block;

    // continue walking
    for (; step < double_block + step_reminder; ++step)
    {
        mark_steps(step);
    }

    long result2 = count_reached_cell(CENTER_OFFSET, 0)
                 + count_reached_cell(CENTER_OFFSET, MULTIPLIER - 1)
                 + count_reached_cell(0, CENTER_OFFSET)
                 + count_reached_cell(MULTIPLIER - 1, CENTER_OFFSET);

    int outer_shell = count_reached_cell(CENTER_OFFSET - 1, 0)
                    + count_reached_cell(CENTER_OFFSET + 1, 0)
                    + count_reached_cell(0, CENTER_OFFSET + 1)
                    + count_reached_cell(MULTIPLIER - 1, CENTER_OFFSET + 1);

    int inner_shell = count_reached_cell(CENTER_OFFSET - 1, 1)
                    + count_reached_cell(CENTER_OFFSET + 1, 1)
                    + count_reached_cell(1, CENTER_OFFSET + 1)
                    + count_reached_cell(MULTIPLIER - 2, CENTER_OFFSET + 1);

    int center_fill = count_reached_cell(CENTER_OFFSET, CENTER_OFFSET);
    int outer_fill  = count_reached_cell(CENTER_OFFSET + 1, CENTER_OFFSET);

    // Formulas were derived by analysis of generated outputs for
    // 2x, 3x and 4x double_block + rem step outputs (see calc_prod.py file)
    // Not sure if this is optimal calculation, but it seems to work.
    long dbc = 4 * (target_dblocks - 1) * target_dblocks;
    long inner_count = 1 + dbc;
    long outer_count = 4 * target_dblocks + dbc;

    result2 += outer_shell * 2 * target_dblocks;
    result2 += inner_shell * (2 * target_dblocks - 1);
    result2 += inner_count * center_fill;
    result2 += outer_count * outer_fill;

    // Result p2: 639051580070841
    printf("Result p2: %ld\n", result2);

    #ifdef DEBUG_PRINT
    for (int cy = 0; cy < MULTIPLIER; ++cy)
    {
        for (int cx = 0; cx < MULTIPLIER; ++cx)
        {
            int creach = count_reached_cell(cy, cx);
            printf("%4d ", creach);
        }
        printf("\n");
    }
    #endif

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
