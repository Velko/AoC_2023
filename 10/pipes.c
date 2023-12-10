#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>


#define BUFFER_SIZE     150 /* input appears to be 140x140, but let's add a bit extra for \n and \0 */



char maze[BUFFER_SIZE][BUFFER_SIZE];

enum direction
{
    UP,
    LEFT,
    DOWN,
    RIGHT,
};

const char *dir_str = "XULDR";

static int move_row(int row, enum direction dir);
static int move_col(int col, enum direction dir);
static bool can_enter(int row, int col, enum direction moving_to);
static enum direction find_start_direction(int row, int col);
static enum direction turn(int row, int col, enum direction moving_to);

int main(void)
{
    FILE *input = fopen("input.txt", "r");

    int num_rows = 0;
    int start_row = -1;
    int start_col = -1;

    for (;; ++num_rows)
    {
        if (fgets(maze[num_rows], BUFFER_SIZE, input) == NULL) break;
        char *s_ptr = strchr(maze[num_rows], 'S');
        if (s_ptr)
        {
            start_row = num_rows;
            start_col = s_ptr - maze[num_rows];
        }
    }

    enum direction dir =  find_start_direction(start_row, start_col);

    printf("Rows: %d\n", num_rows);
    printf("0: (%d, %d) -> %c\n", start_row, start_col, dir_str[dir + 1]);


    int steps = 0;

    int row = start_row;
    int col = start_col;

    for (;;)
    {
        row = move_row(row, dir);
        col = move_col(col, dir);
        dir = turn(row, col, dir);
        ++steps;

        printf("%d: (%d, %d) -> %c\n", steps, row, col, dir_str[dir + 1]);

        if (row == start_row && col == start_col)
            break;
    }

    printf("Result: %d\n", steps / 2);

    fclose(input);
    return 0;
}

static enum direction find_start_direction(int row, int col)
{
    enum direction start_dir;
    for (start_dir = UP; start_dir <= RIGHT; ++start_dir)
    {
        int try_row = move_row(row, start_dir);
        int try_col = move_col(col, start_dir);

        if (can_enter(try_row, try_col, start_dir))
            return start_dir;
    }

    assert("Direction not found");
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

static bool can_enter(int row, int col, enum direction moving_to)
{
    switch (maze[row][col])
    {
    case '|':
        return  moving_to == UP || moving_to == DOWN;
    case '-':
        return moving_to == LEFT || moving_to == RIGHT;
    case 'L':
        return moving_to == LEFT || moving_to == DOWN;
    case 'J':
        return moving_to == RIGHT || moving_to == DOWN;
    case '7':
        return moving_to == RIGHT || moving_to == UP;
    case 'F':
        return moving_to == LEFT || moving_to == UP;
    case 'S':
        return true; // really?
    default:
        return false;
    }
}

/*
    |    -    L    J    7    F    .    S
*/

static enum direction turn(int row, int col, enum direction moving_to)
{
    switch (maze[row][col])
    {
    case '|':
        return moving_to;
    case '-':
        return moving_to;
    case 'L':
        return moving_to == LEFT ? UP : RIGHT;
    case 'J':
        return moving_to == RIGHT ? UP : LEFT;
    case '7':
        return moving_to == RIGHT ? DOWN : LEFT;
    case 'F':
        return moving_to == LEFT ? DOWN : RIGHT;
    case 'S':
        return -1; // does not matter anymore
    }
}