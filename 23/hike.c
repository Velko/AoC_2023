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

enum direction
{
    UP,
    LEFT,
    DOWN,
    RIGHT,
    NUM_DIRECTIONS
};

#define MAX_VERTICES    40

struct vertice
{
    int row;
    int col;
};

struct vertice vertices[MAX_VERTICES];
int nvertices;

int distances[MAX_VERTICES][MAX_VERTICES];

static int get_longest_path();
static int move_row(int row, enum direction dir);
static int move_col(int col, enum direction dir);
static int num_exits(int row, int col);
static int measure_graph();

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

    // printf("%dx%d\n", nrows, ncols);
    // printf("(%d, %d) -> (%d, %d)\n", start_row, start_col, end_row, end_col);

    //for (int r = 0; r < nrows; ++r) printf("%s\n", maze[r]);

    measure_graph();



    //int result1 = get_longest_path();
    // Result p1: 2170
    //rintf("Result p1: %d\n", result1);

    for (int r = 0; r < nrows; ++r) printf("%s\n", maze[r]);

    return 0;
}

static int find_vertice(int row, int col);

static void follow_path(int row, int col, int old_row, int old_col, int source, int distance)
{
    for(;;)
    {
        int new_row, new_col;
        for (enum direction dir = UP; dir < NUM_DIRECTIONS; ++dir)
        {
            new_row = move_row(row, dir);
            new_col = move_col(col, dir);

            if (new_row < 0 || new_row >= nrows || new_col < 0 || new_col >= ncols)
                continue;

            if (maze[new_row][new_col] == '#')
                continue;

            if (new_row == old_row && new_col == old_col)
                continue;
            break;
        }

        ++distance;

        int existing_vertice = find_vertice(new_row, new_col);
        if (existing_vertice >= 0)
        {
            printf("ext (%d, %d) -> (%d, %d) = %d\n",
                vertices[source].row,
                vertices[source].col,
                new_row, new_col,
                distance);
            return;
        }

        int ndirs = num_exits(new_row, new_col);

        if (ndirs > 2)
        {
            assert(nvertices < MAX_VERTICES);
            vertices[nvertices].row = new_row;
            vertices[nvertices].col = new_col;
            printf("add (%d, %d) -> (%d, %d) = %d\n",
                vertices[source].row,
                vertices[source].col,
                new_row, new_col,
                distance);
            maze[new_row][new_col] = nvertices + '0';
            ++nvertices;
            return;
        }
        old_row = row;
        old_col = col;
        row = new_row;
        col = new_col;
    }
}


static int measure_graph()
{
    memset(distances, 0, sizeof(distances));

    vertices[0].row = start_row;
    vertices[0].col = start_col;
    vertices[1].row = end_row;
    vertices[1].col = end_col;
    nvertices = 2;

    maze[start_row][start_col] = '0';
    maze[end_row][end_col] = '1';

    for (int vi = 0; vi < nvertices; ++vi)
    {
        int row = vertices[vi].row;
        int col = vertices[vi].col;

        for (enum direction dir = UP; dir < NUM_DIRECTIONS; ++dir)
        {
            int new_row = move_row(row, dir);
            int new_col = move_col(col, dir);

            if (new_row < 0 || new_row >= nrows || new_col < 0 || new_col >= ncols)
                continue;

            if (maze[new_row][new_col] == '#')
                continue;

            follow_path(new_row, new_col, row, col, vi, 1);
        }
    }

    return 0;
}


static int num_exits(int row, int col)
{
    int nexits = 0;
    for (enum direction dir = UP; dir < NUM_DIRECTIONS; ++dir)
    {
        int new_row = move_row(row, dir);
        int new_col = move_col(col, dir);

        if (new_row < 0 || new_row >= nrows || new_col < 0 || new_col >= ncols)
            continue;

        if (maze[new_row][new_col] != '#')
            ++nexits;
    }

    return nexits;
}

static int find_vertice(int row, int col)
{
    for (int v = 0; v < nvertices; ++v)
    {
        if (vertices[v].row == row && vertices[v].col == col)
            return v;
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


/* ------------ Dijkstra related stuff ------*/


struct state
{
    int row;
    int col;
    int distance;
    enum direction dir;
};

int nqueue;

int visited[BUFFER_SIZE][BUFFER_SIZE][NUM_DIRECTIONS];

static void setup_queue();
static void push_node(struct state *n);
static void pop_node(struct state *top);
static enum direction reverse(enum direction dir);

static int get_longest_path()
{
    memset(visited, 0, sizeof(visited));
    setup_queue();

    struct state start =
    {
        .row = start_row,
        .col = start_col,
        .dir = DOWN,
        .distance = 0,
    };
    push_node(&start);

    int longest_path = 0;

    while (nqueue > 0)
    {
        struct state node;

        pop_node(&node);

        if (node.row == end_row && node.col == end_col)
        {
            if (node.distance > longest_path)
                longest_path = node.distance;
            continue;
        }

        for (enum direction dir = UP; dir < NUM_DIRECTIONS; ++dir)
        {
            if (reverse(node.dir) == dir)
                continue;
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
            new_node.dir = dir;

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

            int *v = &visited[new_node.row][new_node.col][dir];

            if (new_node.distance > *v)
            {
                *v = new_node.distance;
                push_node(&new_node);
            }
        }
    }

    return longest_path;
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
