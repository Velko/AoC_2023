#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#define BUFFER_SIZE     256

#define MAX_INSTRUCTIONS    700

struct instr
{
    char dir;
    long steps;
};

struct instr instructions[MAX_INSTRUCTIONS];
int ninstructions;


#define GROUND_SIZE     400

char ground[GROUND_SIZE][GROUND_SIZE];
long nrows;
long ncols;
long startrow;
long startcol;

#define TEMP_MARKER     '*'
#define BORDER_MARKER   '#'
#define EMPTY_MARKER    '.'

static void parse_line(char *line);
static void measure();
static void prepare_ground();
static void print_ground();
static void mark_border();
static void flood_outside();
static int count_border_and_inside();

int main(void)
{
    char line[BUFFER_SIZE];
    FILE *input = fopen("input.txt", "r");

    for (ninstructions = 0;; ++ninstructions)
    {
        if (fgets(line, BUFFER_SIZE, input) == NULL) break; 
        parse_line(line);
    }
    fclose(input);


    measure();
    return 0;
    prepare_ground();
    mark_border();
    flood_outside();
    print_ground();


    int result = count_border_and_inside();

    // result p1: 40131
    printf("Result: %d\n", result);
    return 0;
}


static void parse_line(char *line)
{
    char *savep;

    strtok_r(line, "#", &savep);
    char *color = strtok_r(NULL, ")", &savep);

    instructions[ninstructions].dir = color[5];

    color[5] = 0;
    instructions[ninstructions].steps = strtol(color, NULL, 16);

    //sscanf(line, "%c %d", &instructions[ninstructions].dir, &instructions[ninstructions].steps);
    //printf("%05lx %c\n", instructions[ninstructions].steps, instructions[ninstructions].dir);
}

static void measure()
{
    long row_min = 100000;
    long row_max = -100000;

    long col_min = 100000;
    long col_max = -100000;

    long row = 0;
    long col = 0;

    // 0 means R, 1 means D, 2 means L, and 3 means U.
    for (int i = 0; i < ninstructions; ++i)
    {
        switch (instructions[i].dir)
        {
        case 'D':
        case '1':
            row += instructions[i].steps;
            if (row > row_max) row_max = row;
            break;
        case 'U':
        case '3':
            row -= instructions[i].steps;
            if (row < row_min) row_min = row;
            break;
        case 'R':
        case '0':
            col += instructions[i].steps;
            if (col > col_max) col_max = col;
            break;
        case 'L':
        case '2':
            col -= instructions[i].steps;
            if (col < col_min) col_min = col;
            break;
        default:
            printf("WTF\n");
            exit(1);
            break;
        }
    }

    // add 1-char border around
    nrows = row_max - row_min + 1 + 2;
    ncols = col_max - col_min + 1 + 2;
    startrow = -row_min + 1;
    startcol = -col_min + 1;

    printf("(%ld, %ld) to (%ld, %ld) (%ldx%ld)\n", row_min, col_min, row_max, col_max, nrows, ncols);
}

static void mark_border()
{
    int row = startrow;
    int col = startcol;

    ground[row][col] = BORDER_MARKER;

    for (int i = 0; i < ninstructions; ++i)
    {
        switch (instructions[i].dir)
        {
        case 'D':
            for (int s = 0; s < instructions[i].steps; ++s)
            {
                ++row;
                ground[row][col] = BORDER_MARKER;
            }
            break;
        case 'U':
            for (int s = 0; s < instructions[i].steps; ++s)
            {
                --row;
                ground[row][col] = BORDER_MARKER;
            }
            break;
        case 'R':
            for (int s = 0; s < instructions[i].steps; ++s)
            {
                ++col;
                ground[row][col] = BORDER_MARKER;
            }
            break;
        case 'L':
            for (int s = 0; s < instructions[i].steps; ++s)
            {
                --col;
                ground[row][col] = BORDER_MARKER;
            }
            break;
        default:
            printf("WTF\n");
            exit(1);
            break;
        }
    }
}

static void prepare_ground()
{
    memset(ground, TEMP_MARKER, sizeof(ground));
    for (int row = 0; row < nrows; ++row)
        ground[row][ncols] = 0;
}

static void print_ground()
{
    for (int row = 0; row < nrows; ++row)
        printf("%s\n", ground[row]);
}

static int count_border_and_inside()
{
    int count = 0;
    for (int row = 0; row < nrows; ++row)
    {
        for (int col = 0; col < ncols; ++col)
        {
            if (ground[row][col] == BORDER_MARKER || ground[row][col] == TEMP_MARKER)
                ++count;
        }
    }

    return count;
}


static bool mark_around(int row, int col);
static void flood_outside()
{
    bool marked;
    ground[0][0] = EMPTY_MARKER;

    do
    {
        marked = false;
        for (int row = 0; row < nrows; ++row)
        {
            for (int col = 0; col < ncols; ++col)
            {
                if (ground[row][col] == EMPTY_MARKER)
                    marked |= mark_around(row, col);
            }
        }
    } while (marked);
}

static bool mark_around(int row, int col)
{
    bool marked = false;

    if (row > 0 && ground[row - 1][col] == TEMP_MARKER)
    {
        ground[row - 1][col] = EMPTY_MARKER;
        marked = true;
    }

    if (col > 0 && ground[row][col - 1] == TEMP_MARKER)
    {
        ground[row][col - 1] = EMPTY_MARKER;
        marked = true;
    }

    if (ground[row + 1][col] == TEMP_MARKER)
    {
        ground[row + 1][col] = EMPTY_MARKER;
        marked = true;
    }

    if (ground[row][col + 1] == TEMP_MARKER)
    {
        ground[row][col + 1] = EMPTY_MARKER;
        marked = true;
    }

    return marked;
}
