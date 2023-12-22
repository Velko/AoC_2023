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

struct brick
{
    struct coord start;
    struct coord end;
};

#define MAX_BRICKS       1500

#define MAX_Z       350
#define MAX_Y        10
#define MAX_X        10

struct state
{
    struct brick bricks[MAX_BRICKS];
    int nbricks;
    bool pile[MAX_Z][MAX_Y][MAX_X];
};

struct state root_state;

static void init_state(struct state *state);
static void parse_line(struct state *state, const char *line);
static bool is_place_occupied(struct state *state, struct brick *brick);
static bool drop_brick(struct state *state, int brick_idx);
static bool can_fall(struct state *state, int brick_idx);
static void set_pile(struct state *state, int brick_idx, bool occupied);
static void sort_bricks(struct state *state);
static int drop_all_bricks(struct state *state, int skip_brick);
static int num_will_fall_if_removed(struct state *state, int brick_idx);

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

    sort_bricks(&root_state);

    drop_all_bricks(&root_state, -1);

    int result1 = 0;
    int result2 = 0;

    for (int c = 0; c < root_state.nbricks; ++c)
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
    struct brick *brick = &state->bricks[state->nbricks];

    int n = sscanf(line, "%d,%d,%d~%d,%d,%d",
        &brick->start.x,
        &brick->start.y,
        &brick->start.z,
        &brick->end.x,
        &brick->end.y,
        &brick->end.z
    );
    assert(n == 6);

    // validate that *start* is always lower
    assert(brick->start.z <= brick->end.z);
    ++state->nbricks;
}

static int drop_all_bricks(struct state *state, int skip_brick)
{
    int fall_count = 0;

    for (int c = 0; c < state->nbricks; ++c)
    {
        if (c == skip_brick) continue;;
        if (drop_brick(state, c))
            ++fall_count;
    }

    return fall_count;
}

static bool drop_brick(struct state *state, int brick_idx)
{
    bool fell = false;

    // clear it from pile before it starts moving (again)
    set_pile(state, brick_idx, false);

    while (can_fall(state, brick_idx))
    {
        --state->bricks[brick_idx].start.z;
        --state->bricks[brick_idx].end.z;
        fell = true;
    }

    // brick has fallen as low as it will go, settle it in
    set_pile(state, brick_idx, true);

    return fell;
}

static void set_pile(struct state *state, int brick_idx, bool occupied)
{
    struct brick *brick = &state->bricks[brick_idx];

    assert(brick->start.z <= brick->end.z);
    assert(brick->end.z < MAX_Z);
    for (int z = brick->start.z; z <= brick->end.z; ++z)
    {
        assert(brick->start.y <= brick->end.y);
        assert(brick->end.y < MAX_Y);
        for (int y = brick->start.y; y <= brick->end.y; ++y)
        {
            assert(brick->start.x <= brick->end.x);
            assert(brick->end.x < MAX_X);
            for (int x = brick->start.x; x <= brick->end.x; ++x)
            {
                state->pile[z][y][x] = occupied;
            }
        }
    }
}

static bool can_fall(struct state *state, int brick_idx)
{
    if (state->bricks[brick_idx].start.z == 1) return false;

    struct brick candidate = state->bricks[brick_idx];
    --candidate.start.z;
    --candidate.end.z;

    return !is_place_occupied(state, &candidate);
}

static bool is_place_occupied(struct state *state, struct brick *brick)
{
    // probably can do smarter check but this should do
    assert(brick->start.y <= brick->end.y);
    for (int y = brick->start.y; y <= brick->end.y; ++y)
    {
        assert(brick->start.x <= brick->end.x);
        for (int x = brick->start.x; x <= brick->end.x; ++x)
        {
            if (state->pile[brick->start.z][y][x])
                return true;
        }
    }

    return false;
}

static int num_will_fall_if_removed(struct state *state, int brick_idx)
{
    // Testing what happens if a brick is removed modifies the overall state,
    // but for next test we need to start from the original again.
    // Therefore, create a copy and test on that instead
    struct state copy;
    memcpy(&copy, state, sizeof(struct state));

    // simply erase the block
    set_pile(&copy, brick_idx, false);

    // and see what happens
    return drop_all_bricks(&copy, brick_idx);
}

static int cmp_bricks_z(const void *a, const void *b);
static void sort_bricks(struct state *state)
{
    qsort(state->bricks, state->nbricks, sizeof(struct brick), cmp_bricks_z);
}

static int cmp_bricks_z(const void *a, const void *b)
{
    return ((const struct brick *)a)->start.z - ((const struct brick *)b)->start.z;
}
