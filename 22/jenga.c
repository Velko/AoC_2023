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

#define MAX_Z       350
#define MAX_Y        10
#define MAX_X        10

struct state
{
    struct cube cubes[MAX_CUBES];
    int ncubes;
    bool pile[MAX_Z][MAX_Y][MAX_X];
};

struct state root_state;

static void init_state(struct state *state);
static void parse_line(struct state *state, const char *line);
static bool is_place_occupied(struct state *state, struct cube *cube);
static bool fall_cube(struct state *state, int cube_idx);
static void sort_cubes(struct state *state);
static void drop_all_cubes(struct state *state);
static int num_will_fall_if_removed(struct state *state, int cube_idx);

int main(void)
{
    char line[BUFFER_SIZE];
    FILE *input = fopen("input.txt", "r");

    init_state(&root_state);
    for (;;)
    {
        if (fgets(line, BUFFER_SIZE, input) == NULL) break; 
        parse_line(&root_state, line);
    }
    fclose(input);

    sort_cubes(&root_state);

    drop_all_cubes(&root_state);

    int result1 = 0;
    int result2 = 0;

    for (int c = 0; c < root_state.ncubes; ++c)
    {
        int fell = num_will_fall_if_removed(&root_state, c);

        if (fell == 0)
            ++result1;

        result2 += fell;
    }

    // result p1: 524
    printf("Result p1: %d\n", result1);
    
    // result p2: 77070
    printf("Result p2: %d\n", result2);


    return 0;
}


static void init_state(struct state *state)
{
    memset(state, 0, sizeof(struct state));
}

static void parse_line(struct state *state, const char *line)
{
    struct cube *cube = &state->cubes[state->ncubes];

    int n = sscanf(line, "%d,%d,%d~%d,%d,%d",
        &cube->start.x,
        &cube->start.y,
        &cube->start.z,
        &cube->end.x,
        &cube->end.y,
        &cube->end.z
    );
    assert(n == 6);

    // validate that *start* is always lower
    assert(cube->start.z <= cube->end.z);
    ++state->ncubes;
}

static void drop_all_cubes(struct state *state)
{
    for (int c = 0; c < state->ncubes; ++c)
        fall_cube(state, c);
}

static bool can_fall(struct state *state, int cube_idx)
{
    if (state->cubes[cube_idx].start.z == 1) return false;

    struct cube candidate = state->cubes[cube_idx];
    --candidate.start.z;
    --candidate.end.z;

    return !is_place_occupied(state, &candidate);
}

static void set_pile(struct state *state, int cube_idx, bool occupied)
{
    struct cube *cube = &state->cubes[cube_idx];

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
                state->pile[z][y][x] = occupied;
            }
        }
    }
}

static bool fall_cube(struct state *state, int cube_idx)
{
    bool fell = false;
    set_pile(state, cube_idx, false);

    while (can_fall(state, cube_idx))
    {
        --state->cubes[cube_idx].start.z;
        --state->cubes[cube_idx].end.z;
        fell = true;
    }

    // cube has fallen as low as it will go, settle it in
    set_pile(state, cube_idx, true);

    return fell;
}

static int num_will_fall_if_removed(struct state *state, int cube_idx)
{
    struct state copy;
    memcpy(&copy, state, sizeof(struct state));

    set_pile(&copy, cube_idx, false);

    int will_fall = 0;

    for (int c = 0; c < copy.ncubes; ++c)
    {
        if (c == cube_idx) continue;

        if (fall_cube(&copy, c))
            ++will_fall;
    }

    return will_fall;
}

static bool is_place_occupied(struct state *state, struct cube *cube)
{
    // probably can do smarter check but this should do
    assert(cube->start.y <= cube->end.y);
    for (int y = cube->start.y; y <= cube->end.y; ++y)
    {
        assert(cube->start.x <= cube->end.x);
        for (int x = cube->start.x; x <= cube->end.x; ++x)
        {
            if (state->pile[cube->start.z][y][x])
                return true;
        }
    }

    return false;
}

static int cmp_cubes_z(const void *a, const void *b);
static void sort_cubes(struct state *state)
{
    qsort(state->cubes, state->ncubes, sizeof(struct cube), cmp_cubes_z);
}

static int cmp_cubes_z(const void *a, const void *b)
{
    return ((const struct cube *)a)->start.z - ((const struct cube *)b)->start.z;
}
