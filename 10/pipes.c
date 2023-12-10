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

static int move_row(int row, enum direction dir);
static int move_col(int col, enum direction dir);
static bool can_enter(int row, int col, enum direction moving_to);
static enum direction find_start_direction(int row, int col);

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

    enum direction start_dir =  find_start_direction(start_row, start_col);

    printf("Rows: %d\n", num_rows);
    printf("Start: (%d, %d) -> %d\n", start_row, start_col, start_dir);


    return 0;


    int total = 0;

    for (;;)
    {
    }

    printf("Result: %d\n", total);

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

/*
    |    -    L    J    7    F    .    S

*/

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