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

#define MATRIX_MAX  5

struct matrix
{
    double matrix[MATRIX_MAX][MATRIX_MAX];
    int nrows;
};


static void parse_line(const char *line);
static bool calc_intersect(struct hail *a, struct hail *b);
static bool gauss_eliminate(struct matrix *m);
static int calc_p1();

#ifdef DEBUG_PRINT
static void print_hail(struct hail *a);
#endif

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

    int result1 = calc_p1();

    // Result p1: 16018
    printf("Result p1: %d\n", result1);

    return 0;
}

static int calc_p1()
{
    int total = 0;

    for (int j = 0; j < nhailstones; ++j)
    {
        for (int i = j + 1; i < nhailstones; ++i)
        {
            if (calc_intersect(hailstones + j, hailstones + i))
                ++total;
        }
    }

    return total;
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

static bool calc_intersect(struct hail *a, struct hail *b)
{
    #ifdef DEBUG_PRINT
    printf("\n");
    print_hail(a);
    print_hail(b);
    #endif

    struct matrix m = {
        .nrows = 2,
        .matrix = { 
            { a->velocity.x, -b->velocity.x, b->position.x - a->position.x },
            { a->velocity.y, -b->velocity.y, b->position.y - a->position.y },
        },
    };

    if (!gauss_eliminate(&m))
        return false;

    double time_a = m.matrix[0][2];
    double time_b = m.matrix[1][2];

    if (time_a < 0 || time_b < 0)
        return false;

    double intersect_x = b->position.x + b->velocity.x * time_b;
    double intersect_y = b->position.y + b->velocity.y * time_b;

    bool fits = intersect_x >= TEST_AREA_X_MIN && intersect_x <= TEST_AREA_X_MAX
             && intersect_y >= TEST_AREA_Y_MIN && intersect_y <= TEST_AREA_Y_MAX;

    #ifdef DEBUG_PRINT
    printf("Intersects %s at %ld (%d), %ld (%d)\n", fits ? "inside" : "outside", intersect_x, rem_x, intersect_y, rem_y);
    #endif

    return fits;
}

static bool gauss_eliminate(struct matrix *m)
{
    int ncols = m->nrows + 1;

    for (int i = 0; i < m->nrows; ++i)
    {
        double d = m->matrix[i][i];
        if (d == 0) return false;

        for (int c = i; c < ncols; ++c)
            m->matrix[i][c] /= d;

        for (int j = i + 1; j < m->nrows; ++j)
        {
            double mul = m->matrix[j][i];
            for (int c = i; c < ncols; ++c)
                m->matrix[j][c] -= m->matrix[i][c] * mul;
        }
    }

    for (int i = m->nrows - 1; i > 0; --i)
    {
        for (int j = 0; j < i; ++j)
        {
            double mul = m->matrix[j][i];
            for (int c = i; c < ncols; ++c)
                m->matrix[j][c] -= m->matrix[i][c] * mul;
        }
    }

    return true;

}

#ifdef DEBUG_PRINT
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
#endif
