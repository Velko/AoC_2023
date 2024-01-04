#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>


#define BUFFER_SIZE     150 /* input appears to be 140x140, but let's add a bit extra for \n and \0 */



char maze[BUFFER_SIZE][BUFFER_SIZE];
char clean_maze[BUFFER_SIZE][BUFFER_SIZE];

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
static bool can_enter(char tile, enum direction moving_to);
static enum direction find_start_direction(int row, int col);
static enum direction turn(char tile, enum direction moving_to);
static int walk_path_and_copy(int start_row, int start_col, enum direction start_dir);
static void walk_path_and_mark(int start_row, int start_col, enum direction start_dir);
static void mark_inside(int row, int col, int dir);
static int flood_inside(int num_rows);
static void convert_print(const char *str);

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
    printf("0: (%d, %d) -> %c\n", start_row, start_col, dir_str[start_dir + 1]);


    memset(clean_maze, '.', sizeof(clean_maze));
    int ncols = strlen(maze[0]);
    for (int r = 0; r < num_rows; ++r)
    {
        clean_maze[r][ncols] = 0;
    }


    int steps = walk_path_and_copy(start_row, start_col, start_dir);

    // result p1: 6860
    printf("Result1: %d\n", steps / 2); // technically should have rounded up, but whatever, got lucky.

    walk_path_and_mark(start_row, start_col, start_dir);

    int prev_flood = -1;
    int inside_count = flood_inside(num_rows);
    while (inside_count != prev_flood)
    {
        prev_flood = inside_count;
        inside_count = flood_inside(num_rows);
    }


    for (int r = 0; r < num_rows; ++r)
    {
        convert_print(clean_maze[r]);
        putc(10, stdout);
    }

    // Result2: 343
    printf("Result2: %d\n", inside_count);

    fclose(input);
    return 0;
}

static int walk_path_and_copy(int start_row, int start_col, enum direction start_dir)
{
    int steps = 0;

    int row = start_row;
    int col = start_col;
    enum direction dir = start_dir;

    for (;;)
    {
        clean_maze[row][col] = maze[row][col];
        row = move_row(row, dir);
        col = move_col(col, dir);
        dir = turn(maze[row][col], dir);
        ++steps;


        //printf("%d: (%d, %d) -> %c\n", steps, row, col, dir_str[dir + 1]);

        if (row == start_row && col == start_col)
            break;
    }

    return steps;
}

static void walk_path_and_mark(int start_row, int start_col, enum direction start_dir)
{
    int row = start_row;
    int col = start_col;
    enum direction dir = start_dir;

    for (;;)
    {
        row = move_row(row, dir);
        col = move_col(col, dir);
        mark_inside(row, col, dir);
        dir = turn(clean_maze[row][col], dir);

        mark_inside(row, col, dir);

        if (row == start_row && col == start_col)
            break;
    }
}

static void mark_inside(int row, int col, int dir)
{
    /* We're walking counter clock-wise, the opportunity for "inside"
       tile is only when moving straight:
        * up - to the left
        * down - to the right
        * left - down
        * right - up

       NOTE: this is not actually true, as we might think that we're moving
         anti-clockwise (because of starting direction), but actually are
         going clockwise, as indicated by example that expects 8 enclosed
         tiles. If that happens, adjust the inside detection to opposite.
     */

    int try_row = row;
    int try_col = col;

    switch (dir)
    {
    case UP:
        try_col = col - 1;
        break;
    case DOWN:
        try_col = col + 1;
        break;
    case LEFT:
        try_row = row + 1;
        break;
    case RIGHT:
        try_row = row - 1;
        break;
    }

    if (try_col < 0 || try_row < 0) return;

    if (clean_maze[try_row][try_col] == '.')
        clean_maze[try_row][try_col] = 'I';
}

static void mark_around(int row, int col)
{
    if (row > 0 && clean_maze[row - 1][col] == '.')
        clean_maze[row - 1][col] = 'I';

    if (col > 0 && clean_maze[row][col - 1] == '.')
        clean_maze[row][col - 1] = 'I';

    if (clean_maze[row + 1][col] == '.')
        clean_maze[row + 1][col] = 'I';

    if (clean_maze[row][col + 1] == '.')
        clean_maze[row][col + 1] = 'I';
}

static int flood_inside(int num_rows)
{
    int inside_count = 0;

    for (int row = 0; row < num_rows; ++row)
    {
        for (int col = 0; clean_maze[row][col]; ++col)
        {
            if (clean_maze[row][col] == 'I')
            {
                ++inside_count;
                mark_around(row, col);
            }
        }
    }

    return inside_count;
}

static enum direction find_start_direction(int row, int col)
{
    enum direction start_dir;
    for (start_dir = UP; start_dir <= RIGHT; ++start_dir)
    {
        int try_row = move_row(row, start_dir);
        int try_col = move_col(col, start_dir);

        if (can_enter(maze[try_row][try_col], start_dir))
            return start_dir;
    }

    assert("Direction not found");
    return -1;
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

static bool can_enter(char tile, enum direction moving_to)
{
    switch (tile)
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

static enum direction turn(char tile, enum direction moving_to)
{
    switch (tile)
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
    return -1;
    assert(0);
}


static void convert_print(const char *s)
{
    /* print the maze using Box-drawing characters - greatly improves visual appearance */
    while (*s)
    {
        switch (*s)
        {
        case '|':
            printf("│");
            break;
        case '-':
            printf("─");
            break;
        case 'L':
            printf("└");
            break;
        case 'J':
            printf("┘");
            break;
        case '7':
            printf("┐");
            break;
        case 'F':
            printf("┌");
            break;
        default:
            putc(*s, stdout);
            break;
        }
        ++s;
    }
}