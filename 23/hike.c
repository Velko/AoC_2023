#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

#define BUFFER_SIZE     150

//#define DEBUG_PRINT
//#define PART1

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


    printf("    |");
    for (int d = 0; d < nvertices; ++d)
        printf("%3d ", d);
    printf("\n");
    printf("----+-----------------------------------\n");

    for (int s = 0; s < nvertices; ++s)
    {
        printf("%3d |", s);
        for (int d = 0; d < nvertices; ++d)
        {
            printf("%3d ", distances[s][d]);
        }
        printf("\n");
    }


    int result1 = get_longest_path();
    // Result p1: 2170
    // Wrong result p2: 6681 (too high)
    printf("Result p2: %d\n", result1);

    #ifdef DEBUG_PRINT
    for (int r = 0; r < nrows; ++r) printf("%s\n", maze[r]);
    #endif

    return 0;
}

static int find_vertice(int row, int col);

static void follow_path(int row, int col, int old_row, int old_col, int source, int distance)
{
    for(;;)
    {
        int new_row, new_col;
        enum direction dir;
        for (dir = UP; dir < NUM_DIRECTIONS; ++dir)
        {
            #ifdef PART1
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
            #endif

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
            printf("ext %d(%d, %d) -> %d(%d, %d) = %d\n",
                source,
                vertices[source].row,
                vertices[source].col,
                existing_vertice,
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
            printf("add %d(%d, %d) -> %d(%d, %d) = %d\n",
                source,
                vertices[source].row,
                vertices[source].col,
                nvertices,
                new_row, new_col,
                distance);
            //maze[new_row][new_col] = nvertices + '0';
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

static int measure_graph()
{
    memset(distances, 0, sizeof(distances));

    vertices[0].row = start_row;
    vertices[0].col = start_col;
    vertices[1].row = end_row;
    vertices[1].col = end_col;
    nvertices = 2;

    //maze[start_row][start_col] = '0';
    //maze[end_row][end_col] = '1';

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

static int walk_nodes(int source, uint64_t history)
{
    int longest = 0;
    int ldest = -1;

    for (int dest = 0; dest < nvertices; ++dest)
    {
        if (history & (1ULL << dest))
            continue;
        if (distances[source][dest] > 0)
        {
            int len = distances[source][dest];
            if (dest != 1)
            {
                len += walk_nodes(dest, history | (1ULL << source));
            }

            if (len > longest)
            {
                longest = len;
                ldest = dest;
            } 
        }
    }
//    printf(" -> %d", ldest);
    return longest;
}

static int get_longest_path()
{
//    printf("0");
    int longest_path = walk_nodes(0, 0);

//    printf("\n");
    return longest_path;
}
