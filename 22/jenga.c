#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <limits.h>

#define BUFFER_SIZE     256

struct coord
{
    int x;
    int y;
    int z;
};

struct cube
{
    struct coord start;
    struct coord end;
};

#define MAX_CUBES       1500

struct cube cubes[MAX_CUBES];
int ncubes;



#define MAX_Z       350
#define MAX_Y        10
#define MAX_X        10

bool pile[MAX_Z][MAX_Y][MAX_X];



static void parse_line(const char *line);
static bool is_place_occupied(struct cube *cube);
static void fall_cube(struct cube *cube);
static void sort_cubes();

int main(void)
{
    char line[BUFFER_SIZE];
    FILE *input = fopen("input.txt", "r");

    ncubes = 0;
    for (;;)
    {
        if (fgets(line, BUFFER_SIZE, input) == NULL) break; 
        parse_line(line);
    }
    fclose(input);

    sort_cubes();

    struct coord min_c = { .x = INT_MAX, .y = INT_MAX, .z = INT_MAX };
    struct coord max_c = { .x = INT_MIN, .y = INT_MIN, .z = INT_MIN };

    for (int c = 0; c < ncubes; ++c)
    {
        printf("(%d, %d, %d)\t[%d, %d, %d]\t",
            cubes[c].start.x,
            cubes[c].start.y,
            cubes[c].start.z,
            cubes[c].end.x - cubes[c].start.x + 1,
            cubes[c].end.y - cubes[c].start.y + 1,
            cubes[c].end.z - cubes[c].start.z + 1
        );

        fall_cube(cubes + c);

        printf("(%d, %d, %d)\n",
            cubes[c].start.x,
            cubes[c].start.y,
            cubes[c].start.z
        );

        if (max_c.x < cubes[c].start.x) max_c.x = cubes[c].start.x;
        if (max_c.y < cubes[c].start.y) max_c.y = cubes[c].start.y;
        if (max_c.z < cubes[c].start.z) max_c.z = cubes[c].start.z;
        if (min_c.x > cubes[c].start.x) min_c.x = cubes[c].start.x;
        if (min_c.y > cubes[c].start.y) min_c.y = cubes[c].start.y;
        if (min_c.z > cubes[c].start.z) min_c.z = cubes[c].start.z;
        if (max_c.x < cubes[c].end.x) max_c.x = cubes[c].end.x;
        if (max_c.y < cubes[c].end.y) max_c.y = cubes[c].end.y;
        if (max_c.z < cubes[c].end.z) max_c.z = cubes[c].end.z;
        if (min_c.x > cubes[c].end.x) min_c.x = cubes[c].end.x;
        if (min_c.y > cubes[c].end.y) min_c.y = cubes[c].end.y;
        if (min_c.z > cubes[c].end.z) min_c.z = cubes[c].end.z;
    }

    printf("(%d, %d, %d) - (%d, %d, %d)\n",
        min_c.x,
        min_c.y,
        min_c.z,
        max_c.x,
        max_c.y,
        max_c.z
    );


    int total = 0;
    printf("Result: %d\n", total);

    
    return 0;
}


static void parse_line(const char *line)
{
    int n = sscanf(line, "%d,%d,%d~%d,%d,%d",
        &cubes[ncubes].start.x,
        &cubes[ncubes].start.y,
        &cubes[ncubes].start.z,
        &cubes[ncubes].end.x,
        &cubes[ncubes].end.y,
        &cubes[ncubes].end.z
    );
    assert(n == 6);

    // validate that *start* is always lower
    assert(cubes[ncubes].start.z <= cubes[ncubes].end.z);
    ++ncubes;
}

static bool can_fall(struct cube *cube)
{
    if (cube->start.z == 1) return false;

    struct cube candidate = *cube;
    --candidate.start.z;
    --candidate.end.z;

    return !is_place_occupied(&candidate);
}

static void set_pile(struct cube *cube, bool occupied)
{
    assert(cube->start.z <= cube->end.z);
    for (int z = cube->start.z; z <= cube->end.z; ++z)
    {
        assert(cube->start.y <= cube->end.y);
        for (int y = cube->start.y; y <= cube->end.y; ++y)
        {
            assert(cube->start.x <= cube->end.x);
            for (int x = cube->start.x; x <= cube->end.x; ++x)
            {
                pile[z][y][x] = occupied;
            }
        }
    }
}

static void fall_cube(struct cube *cube)
{
    while (can_fall(cube))
    {
        --cube->start.z;
        --cube->end.z;
    }

    // cube has fallen as low as it will go, settle it in
    set_pile(cube, true);
}

static bool is_place_occupied(struct cube *cube)
{
    // probably can do smarter check but this should do
    assert(cube->start.y <= cube->end.y);
    for (int y = cube->start.y; y <= cube->end.y; ++y)
    {
        assert(cube->start.x <= cube->end.x);
        for (int x = cube->start.x; x <= cube->end.x; ++x)
        {
            if (pile[cube->start.z][y][x])
                return true;
        }
    }

    return false;
}

static int cmp_cubes_z(const void *a, const void *b);
static void sort_cubes()
{
    qsort(cubes, ncubes, sizeof(struct cube), cmp_cubes_z);
}

static int cmp_cubes_z(const void *a, const void *b)
{
    return ((const struct cube *)a)->start.z - ((const struct cube *)b)->start.z;
}
