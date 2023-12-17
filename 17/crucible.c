#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#define BUFFER_SIZE     150


enum direction
{
    UP,
    LEFT,
    DOWN,
    RIGHT,
    NUM_DIRECTIONS, /* serves as total number and "non-visited" */
    START
};

#define MAX_STEPS       3

char city[BUFFER_SIZE][BUFFER_SIZE];
unsigned visited[BUFFER_SIZE][BUFFER_SIZE][NUM_DIRECTIONS][MAX_STEPS];
int nrows;
int ncols;


struct state
{
    int row;
    int col;
    unsigned heatloss;
    enum direction dir;
    int steps_straight;
};

#define MAX_QUEUE       (BUFFER_SIZE * BUFFER_SIZE)

struct state queue[MAX_QUEUE];
int nqueue;




const char *dir_str = "^<v>.S";

static int move_row(int row, enum direction dir);
static int move_col(int col, enum direction dir);
static enum direction reverse(enum direction);
static void pop_node(struct state *top);
static void push_node(struct state *n);

static unsigned get_least_heatloss();

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


    unsigned result = get_least_heatloss();

    printf("Result: %d\n", result);

    return 0;
}

static unsigned get_least_heatloss()
{
    memset(visited, -1, sizeof(visited));

    queue[0].row = 0;
    queue[0].col = 0;
    queue[0].dir = START;
    queue[0].steps_straight = 0;
    queue[0].heatloss = 0;
    nqueue = 1;

    while (nqueue > 0)
    {
        struct state node;

        pop_node(&node);

        if (node.row == nrows-1 && node.col == ncols-1)
            return node.heatloss;

        for (enum direction dir = UP; dir < NUM_DIRECTIONS; ++dir)
        {
            if (reverse(node.dir) == dir)
                continue;

            struct state new_node;
            new_node.row = move_row(node.row, dir);
            new_node.col = move_col(node.col, dir);
            new_node.dir = dir;

            if (new_node.row < 0 || new_node.row >= nrows || new_node.col < 0 || new_node.col >= ncols)
                continue;

            new_node.steps_straight = dir == node.dir ? node.steps_straight + 1 : 1;

            if (new_node.steps_straight > 3) continue;

            new_node.heatloss = node.heatloss + city[new_node.row][new_node.col] - '0';

            unsigned *v = &visited[new_node.row][new_node.col][new_node.dir][new_node.steps_straight];

            if (new_node.heatloss < *v)
            {
                *v = new_node.heatloss;
                push_node(&new_node);
            }
        }
    }

    return false;
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

static enum direction reverse(enum direction dir)
{
    switch (dir)
    {
    case UP:
        return DOWN;
    case DOWN:
        return UP;
    case LEFT:
        return RIGHT;
    case RIGHT:
        return LEFT;
    default:
        return dir;
    }
}


/* Probably not the best priority queue implementation but 
   should work.
 */
static void pop_node(struct state *top)
{
    assert(nqueue > 0);
    memcpy(top, queue, sizeof(struct state));

    --nqueue;

    memmove(queue, queue + 1, sizeof(struct state) * nqueue);
}

static int compare_states(const void *a, const void *b);
static void push_node(struct state *n)
{
    assert(nqueue < MAX_QUEUE);

    memcpy(queue + nqueue, n, sizeof(struct state));
    ++nqueue;

    qsort(queue, nqueue, sizeof(struct state), compare_states);
}


static int compare_states(const void *a, const void *b)
{
    const struct state *sa = (const struct state *)a;
    const struct state *sb = (const struct state *)b;

    return (int)sa->heatloss - (int)sb->heatloss;
}
