#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#define BUFFER_SIZE     140

char garden[BUFFER_SIZE][BUFFER_SIZE];
int nrows, ncols;

int start_row, start_col;

#define EMPTY_MARKER    '.'
#define REACH_MARKER    'O'
#define ROCK_MARKER     '#'

int visited[BUFFER_SIZE][BUFFER_SIZE];

static void mark_around(int row, int col, int step);
static void mark_steps(int step);
static int count_reached();

int main(void)
{
    FILE *input = fopen("input.txt", "r");

    nrows = ncols = 0;
    for (;;)
    {
        if (fgets(garden[nrows], BUFFER_SIZE, input) == NULL) break;

        if (ncols == 0)
            ncols = strlen(garden[nrows]) - 1;

        char *p = strchr(garden[nrows], 'S');
        if (p)
        {
            start_row = nrows;
            start_col = p - garden[nrows];
        }

        garden[nrows][ncols] = 0; // chomp \n

        ++nrows;
    }
    fclose(input);

    memset(visited, 0, sizeof(visited));

    mark_around(start_row, start_col, 1);

    for (int step = 1; step < 64; ++step)
        mark_steps(step);


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
