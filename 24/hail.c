#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#define BUFFER_SIZE     256

//#define TEST_DATA
//#define DEBUG_PRINT

#ifdef TEST_DATA
#define TEST_AREA_MIN         7
#define TEST_AREA_MAX         27
#else
#define TEST_AREA_MIN         200000000000000
#define TEST_AREA_MAX         400000000000000
#endif

#define TEST_AREA_X_MIN         TEST_AREA_MIN
#define TEST_AREA_Y_MIN         TEST_AREA_MIN

#define TEST_AREA_X_MAX         TEST_AREA_MAX
#define TEST_AREA_Y_MAX         TEST_AREA_MAX


struct point3d
{
    long x;
    long y;
    long z;
};

struct hail
{
    struct point3d position;
    struct point3d velocity;
};

#define MAX_HAIL        300

struct hail hailstones[MAX_HAIL];
int nhailstones;

static void parse_line(const char *line);
static bool calc_intersect(struct hail *a, struct hail *b);
static void print_hail(struct hail *a);

int main(void)
{
    char line[BUFFER_SIZE];
    #ifdef TEST_DATA
    FILE *input = fopen("sample_input.txt", "r");
    #else
    FILE *input = fopen("input.txt", "r");
    #endif

    for (;;)
    {
        if (fgets(line, BUFFER_SIZE, input) == NULL) break; 
        parse_line(line);
    }
    fclose(input);

    int total = 0;

    for (int j = 0; j < nhailstones; ++j)
    {
        for (int i = j + 1; i < nhailstones; ++i)
        {
            if (calc_intersect(hailstones + j, hailstones + i))
                ++total;
        }
    }

    // Wrong Result p1: 14609 (too low)
    // Result p1: 16018 (calculated in Python)
    printf("Result: %d\n", total);

    
    return 0;
}


static void parse_line(const char *line)
{
    struct hail *h = hailstones + nhailstones;

    int n = sscanf(line, "%ld, %ld, %ld @ %ld, %ld, %ld",
        &h->position.x,
        &h->position.y,
        &h->position.z,
        &h->velocity.x,
        &h->velocity.y,
        &h->velocity.z);
    
    assert(n == 6);

    ++nhailstones;
}


static long floor_div(long dividend, long divisor, bool *had_reminder)
{
    long result = dividend / divisor;
    *had_reminder = (dividend % divisor) != 0;
    if (result < 0 && *had_reminder)
        --result;

    return result;
}

static bool calc_intersect(struct hail *a, struct hail *b)
{
    #ifdef DEBUG_PRINT
    printf("\n");
    print_hail(a);
    print_hail(b);
    #endif


    long dividend_b = (b->position.y - a->position.y) * a->velocity.x - (b->position.x - a->position.x) * a->velocity.y;
    long divisor_b = b->velocity.x * a->velocity.y - b->velocity.y * a->velocity.x;

    long dividend_a = (a->position.y - b->position.y) * b->velocity.x - (a->position.x - b->position.x) * b->velocity.y;
    long divisor_a = a->velocity.x * b->velocity.y - a->velocity.y * b->velocity.x;

    if (divisor_b == 0) // parallel
    {
        #ifdef DEBUG_PRINT
        printf("Parallel\n");
        #endif
        return false;
    }

    if ((dividend_b > 0 && divisor_b < 0) || (dividend_b < 0 && divisor_b > 0)) // signs differ, intersected in past
    {
        #ifdef DEBUG_PRINT
        printf("Past B\n");
        #endif
        return false;
    }

    if ((dividend_a > 0 && divisor_a < 0) || (dividend_a < 0 && divisor_a > 0)) // signs differ, intersected in past
    {
        #ifdef DEBUG_PRINT
        printf("Past A\n");
        #endif
        return false;
    }

    bool rem_x, rem_y;
    long intersect_x = b->position.x + floor_div(b->velocity.x * dividend_b, divisor_b, &rem_x);
    long intersect_y = b->position.y + floor_div(b->velocity.y * dividend_b, divisor_b, &rem_y);

    bool fits = intersect_x >= TEST_AREA_X_MIN && (intersect_x < TEST_AREA_X_MAX || (intersect_x == TEST_AREA_X_MAX && !rem_x))
             && intersect_y >= TEST_AREA_Y_MIN && (intersect_y < TEST_AREA_Y_MAX || (intersect_y == TEST_AREA_Y_MAX && !rem_y));

    if (intersect_x >= TEST_AREA_X_MIN - 5 && intersect_x <= TEST_AREA_X_MAX + 5
     && intersect_y >= TEST_AREA_Y_MIN - 5 && intersect_y <= TEST_AREA_Y_MAX + 5
     && !fits)
     {
        printf("Close: (%ld, %ld)\n", intersect_x, intersect_y);
     }


    #ifdef DEBUG_PRINT
    printf("Intersects %s at %ld (%d), %ld (%d)\n", fits ? "inside" : "outside", intersect_x, rem_x, intersect_y, rem_y);
    #endif

    return fits;
}

static void print_hail(struct hail *h)
{
    printf("%ld, %ld, %ld @ %ld, %ld, %ld\n",
        h->position.x,
        h->position.y,
        h->position.z,
        h->velocity.x,
        h->velocity.y,
        h->velocity.z);
}