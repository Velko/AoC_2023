#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define BUFFER_SIZE     120

char grid[BUFFER_SIZE][BUFFER_SIZE];
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


struct hist_item
{
    int row;
    int col;
    enum direction dir;
};

#define HISTORY_MAX     100


static int move_row(int row, enum direction dir);
static int move_col(int col, enum direction dir);
static void walk_path(int start_row, int start_col, enum direction start_dir, struct hist_item *history, int nhist);
static void turn_and_split(enum direction start_dir, char tile, enum direction *out_dir1, enum direction *out_dir2);

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


    struct hist_item history[HISTORY_MAX];

    walk_path(0, 0, RIGHT, history, 0);

    //for (int r = 0; r < nrows; ++r)
    //    printf("'%s'\n", grid[r]);

    int total = 0;
    printf("Result: %d\n", total);

    fclose(input);
    return 0;
}


static void walk_path(int start_row, int start_col, enum direction start_dir, struct hist_item *history, int nhist)
{
    printf("(%d, %d) ", start_row, start_col);
    if (start_row < 0 || start_row >= nrows || start_col < 0 || start_col >= ncols)
    {
        printf("X\n");
        return;
    }

    for (int h = 0; h < nhist; ++h)
    {
        if (history[h].row == start_row && history[h].col == start_col && history[h].dir == start_dir)
        {
            printf("O\n");
            return;
        }
    }

    assert(nhist < HISTORY_MAX);
    history[nhist].row = start_row;
    history[nhist].col = start_col;
    history[nhist].dir = start_dir;
    ++nhist;

    enum direction dir1;
    enum direction dir2;
    turn_and_split(start_dir, grid[start_row][start_col], &dir1, &dir2);
    {
        printf("%c (1)\n", dir_str[dir1]);

        int row1 = move_row(start_row, dir1);
        int col1 = move_col(start_col, dir1);


        walk_path(row1, col1, dir1, history, nhist);
    }

    if (dir1 != dir2) // split
    {
        int row2 = move_row(start_row, dir2);
        int col2 = move_col(start_col, dir2);

        printf("(%d, %d) %c (2)\n", start_row, start_col, dir_str[dir2]);

        struct hist_item split_history[HISTORY_MAX];
        memcpy(split_history, history, nhist * sizeof(struct hist_item));

        walk_path(row2, col2, dir2, split_history, nhist);
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
