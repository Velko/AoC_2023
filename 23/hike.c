#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#define BUFFER_SIZE     150

char maze[BUFFER_SIZE][BUFFER_SIZE];
int nrows, ncols;

int start_row, start_col;
int end_row, end_col;

int visited[BUFFER_SIZE][BUFFER_SIZE];

struct state
{
    int row;
    int col;
    int distance;
};

enum direction
{
    UP,
    LEFT,
    DOWN,
    RIGHT,
    NUM_DIRECTIONS
};


int nqueue;

static int get_longest_path();
static int move_row(int row, enum direction dir);
static int move_col(int col, enum direction dir);
static void setup_queue();
static void push_node(struct state *n);
static void pop_node(struct state *top);

int main(void)
{
    FILE *input = fopen("input.txt", "r");

    nrows = ncols = 0;
    for (;;)
    {
        if (fgets(maze[nrows], BUFFER_SIZE, input) == NULL) break;
        if (ncols == 0)
            ncols = strlen(maze[nrows]) - 1;
        maze[nrows][ncols] = 0;
        ++nrows;
    }
    fclose(input);

    start_row = 0;
    
    char *p = strchr(maze[start_row], '.');
    assert(p != NULL);
    start_col = p -  maze[start_row];

    end_row = nrows - 1;
    p = strchr(maze[end_row], '.');
    assert(p != NULL);
    end_col = p - maze[end_row];

    printf("%dx%d\n", nrows, ncols);
    printf("(%d, %d) -> (%d, %d)\n", start_row, start_col, end_row, end_col);

    for (int r = 0; r < nrows; ++r) printf("%s\n", maze[r]);

    int result1 = get_longest_path();
    printf("Result p1: %d\n", result1);

    for (int r = 0; r < nrows; ++r) printf("%s\n", maze[r]);

    return 0;
}


static int get_longest_path()
{
    memset(visited, 0, sizeof(visited));
    setup_queue();

    struct state start =
    {
        .row = start_row,
        .col = start_col,
        .distance = 0,
    };
    push_node(&start);

    while (nqueue > 0)
    {
        struct state node;

        pop_node(&node);

        if (node.row == end_row && node.col == end_col)
            return node.distance;

        for (enum direction dir = UP; dir < NUM_DIRECTIONS; ++dir)
        {
            switch (maze[node.row][node.col])
            {
            case '>':
                if (dir != RIGHT) continue;
                break;
            case '<':
                if (dir != LEFT) continue;
                break;
            case '^':
                if (dir != UP) continue;
                break;
            case 'v':
                if (dir != DOWN) continue;
                break;
            case '.':
                // just for control
                break;
            default:
                printf("WTF %c\n", maze[node.row][node.col]);
                exit(1);
                break;
            }

            struct state new_node;
            new_node.row = move_row(node.row, dir);
            new_node.col = move_col(node.col, dir);

            if (new_node.row < 0 || new_node.row >= nrows || new_node.col < 0 || new_node.col >= ncols)
                continue;

            switch (maze[new_node.row][new_node.col])
            {
            case '.':
            case '^':
            case 'v':
            case '>':
            case '<':
                break;
            default:
                continue;
            }

            new_node.distance = node.distance + 1;

            int *v = &visited[new_node.row][new_node.col];

            if (new_node.distance > *v)
            {
                *v = new_node.distance;
                push_node(&new_node);
            }
        }
        maze[node.row][node.col] = 'O';
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
    for (i = MAX_BUCKETS-1; i >= 0; --i)
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
    assert(n->distance < MAX_BUCKETS);

    struct state_bucket *bucket = queue + n->distance;

    // check of not full
    assert(bucket->write_idx - bucket->read_idx < MAX_ITEMS_BUCKET);

    int idx = bucket->write_idx % MAX_ITEMS_BUCKET;

    bucket->items[idx] = *n;
    ++bucket->write_idx;
    ++nqueue;
}
