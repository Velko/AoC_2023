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
};

#define MAX_STEPS       10

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

int nqueue;

const char *dir_str = "^<v>.S";

static int move_row(int row, enum direction dir);
static int move_col(int col, enum direction dir);
static enum direction reverse(enum direction);
static void pop_node(struct state *top);
static void push_node(struct state *n);
static void setup_queue();

static unsigned get_least_heatloss(int min_steps, int max_steps);

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


    //unsigned result = get_least_heatloss(1, 3);
    unsigned result = get_least_heatloss(4, 10);

    // result p1: 870
    // result p1: 1063
    printf("Result: %d\n", result);

    return 0;
}

static unsigned get_least_heatloss(int min_steps, int max_steps)
{
    memset(visited, -1, sizeof(visited));
    setup_queue();

    struct state start =
    {
        .row = 0,
        .col = 0,
        .dir = DOWN,
        .heatloss = 0,
        .steps_straight = 0,
    };
    push_node(&start);

    start.dir = RIGHT;
    push_node(&start);


    while (nqueue > 0)
    {
        struct state node;

        pop_node(&node);

        if (node.row == nrows-1 && node.col == ncols-1 && node.steps_straight >= min_steps)
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

            if (dir != node.dir && node.steps_straight < min_steps)
                continue;

            new_node.steps_straight = dir == node.dir ? node.steps_straight + 1 : 1;

            if (new_node.steps_straight > max_steps) continue;

            new_node.heatloss = node.heatloss + city[new_node.row][new_node.col] - '0';

            unsigned *v = &visited[new_node.row][new_node.col][new_node.dir][new_node.steps_straight];

            if (new_node.heatloss < *v)
            {
                *v = new_node.heatloss;
                push_node(&new_node);
            }
        }
    }

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

/* Implement priority queue as buckets of ring buffers */

#define MAX_ITEMS_BUCKET             2048 /* determined experimentally */
#define MAX_BUCKETS                  (BUFFER_SIZE * BUFFER_SIZE)

struct state_bucket
{
    int read_idx;
    int write_idx;
    struct state items[MAX_ITEMS_BUCKET];
};

struct state_bucket queue[MAX_BUCKETS];

static void setup_queue()
{
    memset(queue, 0, sizeof(queue));
    nqueue = 0;
}

static void pop_node(struct state *top)
{
    int i;
    for (i = 0; i < MAX_BUCKETS; ++i)
    {
        if (queue[i].write_idx > queue[i].read_idx)
        {
            int idx = queue[i].read_idx % MAX_ITEMS_BUCKET;
            *top = queue[i].items[idx];
            ++queue[i].read_idx;
            --nqueue;
            return;
        }
    }

    assert(i < MAX_BUCKETS);
}

static void push_node(struct state *n)
{
    assert(n->heatloss < MAX_BUCKETS);

    struct state_bucket *bucket = queue + n->heatloss;

    // check of not full
    assert(bucket->write_idx - bucket->read_idx < MAX_ITEMS_BUCKET);

    int idx = bucket->write_idx % MAX_ITEMS_BUCKET;

    bucket->items[idx] = *n;
    ++bucket->write_idx;
    ++nqueue;
}
