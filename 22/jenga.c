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
static bool fall_cube(int cube_idx);
static void sort_cubes();
static void measure();
static int num_will_fall_if_removed(int cube_idx);
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

    measure();

    sort_cubes();

    // initial fall
    for (int c = 0; c < ncubes; ++c)
    {
        fall_cube(c);
    }

    int result1 = 0;
    int result2 = 0;

    for (int c = 0; c < ncubes; ++c)
    {
        int fell = num_will_fall_if_removed(c);
        if (fell == 0)
        {
            ++result1;
        }
        result2 += fell;
    }

    // result p1: 524
    printf("Result p1: %d\n", result1);
    
    // result p2: 77070
    printf("Result p2: %d\n", result2);


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

static bool can_fall(int cube_idx)
{
    if (cubes[cube_idx].start.z == 1) return false;

    struct cube candidate = cubes[cube_idx];
    --candidate.start.z;
    --candidate.end.z;

    return !is_place_occupied(&candidate);
}

static void set_pile(int cube_idx, bool occupied)
{
    struct cube *cube = &cubes[cube_idx];

    assert(cube->start.z <= cube->end.z);
    assert(cube->end.z < MAX_Z);
    for (int z = cube->start.z; z <= cube->end.z; ++z)
    {
        assert(cube->start.y <= cube->end.y);
        assert(cube->end.y < MAX_Y);
        for (int y = cube->start.y; y <= cube->end.y; ++y)
        {
            assert(cube->start.x <= cube->end.x);
            assert(cube->end.x < MAX_X);
            for (int x = cube->start.x; x <= cube->end.x; ++x)
            {
                pile[z][y][x] = occupied;
            }
        }
    }
}

static bool fall_cube(int cube_idx)
{
    bool fell = false;
    set_pile(cube_idx, false);

    while (can_fall(cube_idx))
    {
        --cubes[cube_idx].start.z;
        --cubes[cube_idx].end.z;
        fell = true;
    }

    // cube has fallen as low as it will go, settle it in
    set_pile(cube_idx, true);

    return fell;
}

static int num_will_fall_if_removed(int cube_idx)
{
    bool backup_pile[MAX_Z][MAX_Y][MAX_X];
    struct cube backup_cubes[MAX_CUBES];

    memcpy(backup_pile, pile, sizeof(pile));
    memcpy(backup_cubes, cubes, sizeof(cubes));

    set_pile(cube_idx, false);

    int will_fall = 0;

    for (int c = 0; c < ncubes; ++c)
    {
        if (c != cube_idx)
            if (fall_cube(c))
                ++will_fall;
    }

    memcpy(pile, backup_pile, sizeof(pile));
    memcpy(cubes, backup_cubes, sizeof(cubes));

    return will_fall;
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

static void measure()
{
    struct coord min_c = { .x = INT_MAX, .y = INT_MAX, .z = INT_MAX };
    struct coord max_c = { .x = INT_MIN, .y = INT_MIN, .z = INT_MIN };

    for (int c = 0; c < ncubes; ++c)
    {
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
}