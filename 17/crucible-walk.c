#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define BUFFER_SIZE     150


enum direction
{
    UP,
    LEFT,
    DOWN,
    RIGHT,
    NUM_DIRECTIONS /* serves as total number and */
};

struct visit_item
{
    int heatloss;
    int nstraight;
    enum direction dir_straight;
};

char city[BUFFER_SIZE][BUFFER_SIZE];
struct visit_item visited[BUFFER_SIZE][BUFFER_SIZE];
int nrows;
int ncols;

const char *dir_str = "^<v>";

static int move_row(int row, enum direction dir);
static int move_col(int col, enum direction dir);
static void walk_path(int row, int col, enum direction dir, struct visit_item *prev);
static void print_visited();

int main(void)
{
    FILE *input = fopen("input.txt", "r");
    nrows = 0;
    ncols = 0;

    for (;;++nrows)
    {
        if (fgets(city[nrows], BUFFER_SIZE, input) == NULL) break; 
        if (ncols == 0)
            ncols = strlen(city[nrows]) - 1;
        city[nrows][ncols] = 0;
    }
    fclose(input);

    printf("%dx%d\n", nrows, ncols);

    for (int row = 0; row < nrows; ++row)
    {
        for (int col = 0; col < ncols; ++col)
        {
            visited[row][col].heatloss = __INT32_MAX__;
            visited[row][col].nstraight = 0;
            visited[row][col].dir_straight = NUM_DIRECTIONS;
        }
    }

    struct visit_item pseudo_prev = { .heatloss = 0, .nstraight = 0, .dir_straight = NUM_DIRECTIONS };

    walk_path(0, 0, NUM_DIRECTIONS, &pseudo_prev);

    printf("Result: %d\n", visited[nrows-1][ncols-1].heatloss);

    return 0;
}


static void walk_path(int row, int col, enum direction dir, struct visit_item *prev)
{
    if (row < 0 || row >= nrows || col < 0 || col >= ncols)
    {
        printf("(%d, %d) X\n", row, col);
        return;
    }

    int curr_heatloss = city[row][col];

    if (visited[row][col].heatloss < prev->heatloss + curr_heatloss)
    {
        printf("(%d, %d) H\n", row, col);
        return;
    }

    if (dir == prev->dir_straight && prev->nstraight >= 3)
    {
        printf("(%d, %d) S\n", row, col);
        return;
    }

    if (dir == prev->dir_straight)
        visited[row][col].nstraight =  prev->nstraight + 1;
    else
        visited[row][col].nstraight = 1;

    visited[row][col].dir_straight = dir;
    visited[row][col].heatloss = prev->heatloss + curr_heatloss;

    print_visited();

    for (enum direction new_dir = UP; new_dir < NUM_DIRECTIONS; ++new_dir)
    {
        printf("(%d, %d) %c\n", row, col, dir_str[new_dir]);
        int new_row = move_row(row, new_dir);
        int new_col = move_col(col, new_dir);
        walk_path(new_row, new_col, new_dir, &visited[row][col]);
    }
}


static int move_row(int row, enum direction dir)
{
    switch (dir)
    {
    case UP:
        return row - 1;
    case DOWN:
        return row + 1;
    default:
        return row;
    }
}

static int move_col(int col, enum direction dir)
{
    switch (dir)
    {
    case LEFT:
        return col - 1;
    case RIGHT:
        return col + 1;
    default:
        return col;
    }
}

static void print_visited()
{
    for (int row = 0; row < nrows; ++row)
    {
        for (int col = 0; col < ncols; ++col)
        {
            enum direction visit = visited[row][col].dir_straight;
            printf("%c", visit != NUM_DIRECTIONS ? dir_str[visit] : '.');
        }
        printf("\n");
    }
    printf("\n");
}