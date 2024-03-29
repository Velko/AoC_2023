#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

#define BUFFER_SIZE     150

//#define DEBUG_PRINT

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

#define MAX_VERTICES    36
const char *vertice_labels = "AZBCDEFGHIJKLMNOPQRSTUVWXY0123456789";
struct vertex
{
    int row;
    int col;
};

#define START_VERTEX     0
#define END_VERTEX       1

struct vertex vertices[MAX_VERTICES];
int nvertices;

int distances[MAX_VERTICES][MAX_VERTICES];

static int get_longest_path();
static int move_row(int row, enum direction dir);
static int move_col(int col, enum direction dir);
static int num_exits(int row, int col);
static void measure_graph(bool respect_slopes);

#ifdef DEBUG_PRINT
static void print_distance_matrix();
#endif

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

    #ifdef DEBUG_PRINT
    printf("%dx%d\n", nrows, ncols);
    printf("A (%d, %d) -> Z (%d, %d)\n", start_row, start_col, end_row, end_col);
    #endif

    measure_graph(true);

    #ifdef DEBUG_PRINT
    print_distance_matrix();
    #endif

    int result1 = get_longest_path();
    // Result p1: 2170
    printf("Result p1: %d\n", result1);


    measure_graph(false);

    #ifdef DEBUG_PRINT
    print_distance_matrix();
    #endif

    int result2 = get_longest_path();
    // Result p2: 6502
    printf("Result p2: %d\n", result2);

    #ifdef DEBUG_PRINT
    for (int r = 0; r < nrows; ++r) printf("%s\n", maze[r]);
    #endif

    return 0;
}

static int find_vertice(int row, int col);

static void follow_path(int row, int col, int old_row, int old_col, int source, int distance, bool respect_slopes)
{
    for(;;)
    {
        int new_row, new_col;
        enum direction dir;
        for (dir = UP; dir < NUM_DIRECTIONS; ++dir)
        {
            if (respect_slopes)
            {
                switch (maze[row][col])
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
                }
            }

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

        if (dir == NUM_DIRECTIONS)
            return;

        ++distance;

        int existing_vertice = find_vertice(new_row, new_col);
        if (existing_vertice >= 0)
        {
            #ifdef DEBUG_PRINT
            printf("ext %c (%d, %d) -> %c (%d, %d) = %d\n",
                vertice_labels[source],
                vertices[source].row,
                vertices[source].col,
                vertice_labels[existing_vertice],
                new_row, new_col,
                distance);
            #endif
            distances[source][existing_vertice] = distance;
            return;
        }

        int ndirs = num_exits(new_row, new_col);

        if (ndirs > 2)
        {
            assert(nvertices < MAX_VERTICES);
            vertices[nvertices].row = new_row;
            vertices[nvertices].col = new_col;
            distances[source][nvertices] = distance;
            #ifdef DEBUG_PRINT
            maze[new_row][new_col] = vertice_labels[nvertices];
            printf("add %c (%d, %d) -> %c (%d, %d) = %d\n",
                vertice_labels[source],
                vertices[source].row,
                vertices[source].col,
                vertice_labels[nvertices],
                new_row, new_col,
                distance);
            #endif
            ++nvertices;
            return;
        }
        old_row = row;
        old_col = col;
        row = new_row;
        col = new_col;
    }
}

static void measure_graph(bool respect_slopes)
{
    memset(distances, 0, sizeof(distances));

    vertices[START_VERTEX].row = start_row;
    vertices[START_VERTEX].col = start_col;
    vertices[END_VERTEX].row = end_row;
    vertices[END_VERTEX].col = end_col;
    nvertices = 2;

    #ifdef DEBUG_PRINT
    maze[start_row][start_col] = vertice_labels[START_VERTEX];
    maze[end_row][end_col] = vertice_labels[END_VERTEX];
    #endif

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

            follow_path(new_row, new_col, row, col, vi, 1, respect_slopes);
        }
    }
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

static int walk_nodes(int source, int dist_so_far, uint64_t history)
{
    int longest = 0;
    for (int dest = 0; dest < nvertices; ++dest)
    {
        if (history & (1ULL << dest))
            continue;

        int len = distances[source][dest];
        if (len > 0)
        {
            if (dest == END_VERTEX)
            {
                return dist_so_far + len;
            }
            int sub_len = walk_nodes(dest, dist_so_far + len, history | (1ULL << source));
            if (sub_len > longest)
                longest = sub_len;
        }
    }

    return longest;
}


static int get_longest_path()
{
    return walk_nodes(START_VERTEX, 0, 0);
}


#ifdef DEBUG_PRINT
static void print_distance_matrix()
{
    printf("    |");
    for (int d = 0; d < nvertices; ++d)
        printf("  %c ", vertice_labels[d]);
    printf("\n");
    printf("----+-----------------------------------\n");

    for (int s = 0; s < nvertices; ++s)
    {
        printf("  %c |", vertice_labels[s]);
        for (int d = 0; d < nvertices; ++d)
        {
            if (s != d)
                printf("%3d ", distances[s][d]);
            else
                printf("  * ");
        }
        printf("\n");
    }
}
#endif
