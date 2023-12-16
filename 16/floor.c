#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define BUFFER_SIZE     120

char grid[BUFFER_SIZE][BUFFER_SIZE];
char visited[BUFFER_SIZE][BUFFER_SIZE];
int nrows;
int ncols;

enum direction
{
    UP,
    LEFT,
    DOWN,
    RIGHT,
};

const char *dir_str = "^<v>";


static int move_row(int row, enum direction dir);
static int move_col(int col, enum direction dir);
static void walk_path(int start_row, int start_col, enum direction start_dir);
static void turn_and_split(enum direction start_dir, char tile, enum direction *out_dir1, enum direction *out_dir2);
static int calculate_energized(int start_row, int start_col, enum direction start_dir);

int main(void)
{
    FILE *input = fopen("input.txt", "r");

    nrows = 0;
    for (;;++nrows)
    {
        if (fgets(grid[nrows], BUFFER_SIZE, input) == NULL) break; 
        if (ncols == 0)
            ncols = strlen(grid[nrows]) - 1;
        grid[nrows][ncols] = 0;
    }
    fclose(input);

    int result = calculate_energized(0, 0, RIGHT);

    // result p1: 7517
    printf("Result: %d\n", result);

    return 0;
}

static int calculate_energized(int start_row, int start_col, enum direction start_dir)
{
    memset(visited, 0, sizeof(visited));
    walk_path(start_row, start_col, start_dir);

    int n_energized = 0;
    for (int r = 0; r < nrows; ++r)
    {
        for (int c = 0; c < ncols; ++c)
        {
            n_energized += visited[r][c] != 0;
        }
    }

    return n_energized;
}


static void walk_path(int start_row, int start_col, enum direction start_dir)
{
    //printf("(%d, %d) ", start_row, start_col);
    if (start_row < 0 || start_row >= nrows || start_col < 0 || start_col >= ncols)
    {
        //printf("X\n");
        return;
    }

    if (visited[start_row][start_col] & (1 << start_dir))
        return;

    visited[start_row][start_col] |= 1 << start_dir;

    enum direction dir1;
    enum direction dir2;
    turn_and_split(start_dir, grid[start_row][start_col], &dir1, &dir2);
    {
        //printf("%c (1)\n", dir_str[dir1]);

        int row1 = move_row(start_row, dir1);
        int col1 = move_col(start_col, dir1);


        walk_path(row1, col1, dir1);
    }

    if (dir1 != dir2) // split
    {
        int row2 = move_row(start_row, dir2);
        int col2 = move_col(start_col, dir2);

        //printf("(%d, %d) %c (2)\n", start_row, start_col, dir_str[dir2]);

        walk_path(row2, col2, dir2);
    }
}


static void turn_and_split(enum direction start_dir, char tile, enum direction *out_dir1, enum direction *out_dir2)
{
    switch (tile)
    {
    case '.':
        *out_dir1 = start_dir;
        *out_dir2 = start_dir;
        break;
    case '/':
        switch (start_dir)
        {
        case LEFT:
            *out_dir1 = DOWN;
            break;
        case RIGHT:
            *out_dir1 = UP;
            break;
        case UP:
            *out_dir1 = RIGHT;
            break;
        case DOWN:
            *out_dir1 = LEFT;
            break;
        default:
            assert(!"WTF!");
            break;
        }
        *out_dir2 = *out_dir1;
        break;
    case '\\':
        switch (start_dir)
        {
        case LEFT:
            *out_dir1 = UP;
            break;
        case RIGHT:
            *out_dir1 = DOWN;
            break;
        case UP:
            *out_dir1 = LEFT;
            break;
        case DOWN:
            *out_dir1 = RIGHT;
            break;
        default:
            assert(!"WTF!");
            break;
        }
        *out_dir2 = *out_dir1;
        break;
    case '-':
        switch (start_dir)
        {
        case LEFT:
        case RIGHT:
            *out_dir1 = start_dir;
            *out_dir2 = start_dir;
            break;
        case UP:
        case DOWN:
            *out_dir1 = LEFT;
            *out_dir2 = RIGHT;
            break;
        default:
            assert(!"WTF!");
            break;
        }
        break;
    case '|':
        switch (start_dir)
        {
        case UP:
        case DOWN:
            *out_dir1 = start_dir;
            *out_dir2 = start_dir;
            break;
        case LEFT:
        case RIGHT:
            *out_dir1 = UP;
            *out_dir2 = DOWN;
            break;
        default:
            assert(!"WTF!");
            break;
        }
        break;
    default:
        assert(!"WTF!");
        break;
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
