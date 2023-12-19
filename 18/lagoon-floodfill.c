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


#define GROUND_SIZE     600

char ground[GROUND_SIZE][GROUND_SIZE];

#define TEMP_MARKER     '*'
#define BORDER_MARKER   '#'
#define EMPTY_MARKER    '.'

#define MAX_RULER       600

struct ruler
{
    long marks[MAX_RULER];
    int nmarks;
};

struct ruler ruler_x;
struct ruler ruler_y;

static void parse_line(char *line);
static void measure();
static void prepare_ground();
static void print_ground();
static void mark_border();
static void flood_outside();
static long count_border_and_inside();

static int ruler_find_mark(struct ruler *ruler, long value);
static void ruler_add_mark(struct ruler *ruler, long value);
static void ruler_init(struct ruler *ruler);
static void ruler_sort(struct ruler *ruler);
static void ruler_print(struct ruler *ruler);
static long ruler_mark_increment(struct ruler *ruler, int mark);

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
    prepare_ground();
    mark_border();
    flood_outside();
    print_ground();
    long result = count_border_and_inside();

    // result p1: 40131
    // result p2: 104454050898331
    printf("Result: %ld\n", result);
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

    //sscanf(line, "%c %ld", &instructions[ninstructions].dir, &instructions[ninstructions].steps);
    //printf("%05lx %c\n", instructions[ninstructions].steps, instructions[ninstructions].dir);
}

static int cmp_long(const void *a, const void *b);


static void measure()
{
    ruler_init(&ruler_x);
    ruler_init(&ruler_y);

    long x = 0;
    long y = 0;


    ruler_add_mark(&ruler_x, x);
    ruler_add_mark(&ruler_y, y);

    // 0 means R, 1 means D, 2 means L, and 3 means U.
    for (int i = 0; i < ninstructions; ++i)
    {
        switch (instructions[i].dir)
        {
        case 'D':
        case '1':
            y += instructions[i].steps;
            ruler_add_mark(&ruler_y, y);
            ruler_add_mark(&ruler_y, y - 1);
            break;
        case 'U':
        case '3':
            y -= instructions[i].steps;
            ruler_add_mark(&ruler_y, y);
            ruler_add_mark(&ruler_y, y - 1);
            break;
        case 'R':
        case '0':
            x += instructions[i].steps;
            ruler_add_mark(&ruler_x, x);
            ruler_add_mark(&ruler_x, x - 1);
            break;
        case 'L':
        case '2':
            x -= instructions[i].steps;
            ruler_add_mark(&ruler_x, x);
            ruler_add_mark(&ruler_x, x - 1);
            break;
        default:
            printf("WTF\n");
            exit(1);
            break;
        }
    }

    ruler_sort(&ruler_x);
    ruler_sort(&ruler_y);

    //printf("Rows: %d\n", ruler_y.nmarks);
    // ruler_print(&ruler_y);

    //printf("Cols: %d\n", ruler_x.nmarks);
    // ruler_print(&ruler_x);
}

static void ruler_init(struct ruler *ruler)
{
    ruler->nmarks = 0;
}

static void ruler_add_mark(struct ruler *ruler, long value)
{
    assert(ruler->nmarks < MAX_RULER);

    for (int i = 0; i < ruler->nmarks; ++i)
    {
        if (ruler->marks[i] == value) return;
    }

    ruler->marks[ruler->nmarks] = value;
    ++ruler->nmarks;
}

static void ruler_sort(struct ruler *ruler)
{
    qsort(ruler->marks, ruler->nmarks, sizeof(long), cmp_long);
}

static int ruler_find_mark(struct ruler *ruler, long value)
{
    long *p = bsearch(&value, ruler->marks, ruler->nmarks, sizeof(long), cmp_long);
    assert (p != NULL);

    return p - ruler->marks;
}

static void ruler_print(struct ruler *ruler)
{
    for (int i = 0; i < ruler->nmarks; ++i)
        printf("%ld, ", ruler->marks[i]);
    printf("\n");
}

static int cmp_long(const void *a, const void *b)
{
    long *la = (long *)a;
    long *lb = (long *)b;

    /* input is long but function has to return int,
       simple subtraction loses bits somewhere
     */
    long diff = *la - *lb;
    if (diff < 0)
        return -1;
    else if (diff > 0)
        return 1;
    else
        return 0;
}

static long ruler_mark_increment(struct ruler *ruler, int mark)
{
    assert(mark > 0 && mark < ruler->nmarks);

    return ruler->marks[mark] - ruler->marks[mark - 1];
}


static void mark_border()
{
    long x = 0;
    long y = 0;

    int row = ruler_find_mark(&ruler_y, y);
    int col = ruler_find_mark(&ruler_x, x);

    ground[row][col] = BORDER_MARKER;

    int new_row, new_col;
    // 0 means R, 1 means D, 2 means L, and 3 means U.
    for (int i = 0; i < ninstructions; ++i)
    {
        //printf("%c %ld -> ", instructions[i].dir, instructions[i].steps);

        int row = ruler_find_mark(&ruler_y, y);
        int col = ruler_find_mark(&ruler_x, x);

        switch (instructions[i].dir)
        {
        case 'D':
        case '1':
            y += instructions[i].steps;
            new_row = ruler_find_mark(&ruler_y, y); 
            //printf("%ld %d\n", y, new_row);
            for (int s = row; s <= new_row; ++s)
            {
                ground[s][col] = BORDER_MARKER;
            }
            break;
        case 'U':
        case '3':
            y -= instructions[i].steps;
            new_row = ruler_find_mark(&ruler_y, y); 
            //printf("%ld %d\n", y, new_row);
            for (int s = row; s >= new_row; --s)
            {
                ground[s][col] = BORDER_MARKER;
            }
            break;
        case 'R':
        case '0':
            x += instructions[i].steps;
            new_col = ruler_find_mark(&ruler_x, x); 
            //printf("%ld %d\n", x, new_col);
            for (int s = col; s <= new_col; ++s)
            {
                ground[row][s] = BORDER_MARKER;
            }
            break;
        case 'L':
        case '2':
            x -= instructions[i].steps;
            new_col = ruler_find_mark(&ruler_x, x); 
            //printf("%ld %d\n", x, new_col);
            for (int s = col; s >= new_col; --s)
            {
                ground[row][s] = BORDER_MARKER;
            }
            break;
        default:
            printf("WTF\n");
            exit(1);
            break;
        }

        //print_ground();
    }
}

static void prepare_ground()
{
    memset(ground, TEMP_MARKER, sizeof(ground));
    for (int row = 0; row <= ruler_y.nmarks; ++row)
        ground[row][ruler_x.nmarks + 1] = 0;
}

static void print_ground()
{
    for (int row = 0; row <= ruler_y.nmarks; ++row)
        printf("%s\n", ground[row]);
    printf("\n");
}

static long count_border_and_inside()
{
    long count = 0;
    for (int row = 0; row < ruler_y.nmarks; ++row)
    {
        for (int col = 0; col < ruler_x.nmarks; ++col)
        {
            if (ground[row][col] == BORDER_MARKER || ground[row][col] == TEMP_MARKER)
            {
                //printf("(%d, %d) -> %ldx%ld\n", row, col, ruler_mark_increment(&ruler_y, row), ruler_mark_increment(&ruler_x, col));
                count += ruler_mark_increment(&ruler_y, row) * ruler_mark_increment(&ruler_x, col);
            }
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
        for (int row = 0; row <= ruler_y.nmarks; ++row)
        {
            for (int col = 0; col <= ruler_x.nmarks; ++col)
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
