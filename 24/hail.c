#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <math.h>

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
    long double matrix[MATRIX_MAX][MATRIX_MAX];
    int nrows;
};


static void parse_line(const char *line);
static bool calc_intersect(struct hail *a, struct hail *b);
static bool gauss_eliminate(struct matrix *m);
static int calc_p1();
static long calc_p2();

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

    long result2 = calc_p2();

    // Result p2: 16018
    printf("Result p2: %ld\n", result2);


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
    {
        #ifdef DEBUG_PRINT
        printf("No solution, probably parallel or standstill\n");
        #endif
        return false;
    }

    double time_a = m.matrix[0][2];
    double time_b = m.matrix[1][2];

    double intersect_x = b->position.x + b->velocity.x * time_b;
    double intersect_y = b->position.y + b->velocity.y * time_b;

    if (time_a < 0 || time_b < 0)
    {
        #ifdef DEBUG_PRINT
        printf("Intersected in past (%f and %f) at (%f, %f)\n", time_a, time_b, intersect_x, intersect_y);
        #endif
        return false;
    }

    bool fits = intersect_x >= TEST_AREA_X_MIN && intersect_x <= TEST_AREA_X_MAX
             && intersect_y >= TEST_AREA_Y_MIN && intersect_y <= TEST_AREA_Y_MAX;

    #ifdef DEBUG_PRINT
    printf("Intersects %s at (%f, %f)\n", fits ? "inside" : "outside", intersect_x, intersect_y);
    #endif

    return fits;
}

static long calc_p2()
{
    struct hail *a = &hailstones[0];
    struct hail *b = &hailstones[1];
    struct hail *c = &hailstones[2];
    struct hail *d = &hailstones[3];
    struct hail *e = &hailstones[4];


    // let's do the calculations around all combinations of axis and average them to improve accuracy
    struct matrix m_xy = {
        .nrows = 4,
        .matrix = {
            {b->velocity.y - a->velocity.y, a->velocity.x - b->velocity.x, a->position.y - b->position.y, b->position.x - a->position.x, b->position.x * b->velocity.y - b->position.y * b->velocity.x - a->position.x * a->velocity.y + a->position.y * a->velocity.x},
            {c->velocity.y - a->velocity.y, a->velocity.x - c->velocity.x, a->position.y - c->position.y, c->position.x - a->position.x, c->position.x * c->velocity.y - c->position.y * c->velocity.x - a->position.x * a->velocity.y + a->position.y * a->velocity.x},
            {d->velocity.y - a->velocity.y, a->velocity.x - d->velocity.x, a->position.y - d->position.y, d->position.x - a->position.x, d->position.x * d->velocity.y - d->position.y * d->velocity.x - a->position.x * a->velocity.y + a->position.y * a->velocity.x},
            {e->velocity.y - a->velocity.y, a->velocity.x - e->velocity.x, a->position.y - e->position.y, e->position.x - a->position.x, e->position.x * e->velocity.y - e->position.y * e->velocity.x - a->position.x * a->velocity.y + a->position.y * a->velocity.x},
        }
    };

    struct matrix m_xz = {
        .nrows = 4,
        .matrix = {
            {b->velocity.z - a->velocity.z, a->velocity.x - b->velocity.x, a->position.z - b->position.z, b->position.x - a->position.x, b->position.x * b->velocity.z - b->position.z * b->velocity.x - a->position.x * a->velocity.z + a->position.z * a->velocity.x},
            {c->velocity.z - a->velocity.z, a->velocity.x - c->velocity.x, a->position.z - c->position.z, c->position.x - a->position.x, c->position.x * c->velocity.z - c->position.z * c->velocity.x - a->position.x * a->velocity.z + a->position.z * a->velocity.x},
            {d->velocity.z - a->velocity.z, a->velocity.x - d->velocity.x, a->position.z - d->position.z, d->position.x - a->position.x, d->position.x * d->velocity.z - d->position.z * d->velocity.x - a->position.x * a->velocity.z + a->position.z * a->velocity.x},
            {e->velocity.z - a->velocity.z, a->velocity.x - e->velocity.x, a->position.z - e->position.z, e->position.x - a->position.x, e->position.x * e->velocity.z - e->position.z * e->velocity.x - a->position.x * a->velocity.z + a->position.z * a->velocity.x},
        }
    };

    struct matrix m_yz = {
        .nrows = 4,
        .matrix = {
            {b->velocity.z - a->velocity.z, a->velocity.y - b->velocity.y, a->position.z - b->position.z, b->position.y - a->position.y, b->position.y * b->velocity.z - b->position.z * b->velocity.y - a->position.y * a->velocity.z + a->position.z * a->velocity.y},
            {c->velocity.z - a->velocity.z, a->velocity.y - c->velocity.y, a->position.z - c->position.z, c->position.y - a->position.y, c->position.y * c->velocity.z - c->position.z * c->velocity.y - a->position.y * a->velocity.z + a->position.z * a->velocity.y},
            {d->velocity.z - a->velocity.z, a->velocity.y - d->velocity.y, a->position.z - d->position.z, d->position.y - a->position.y, d->position.y * d->velocity.z - d->position.z * d->velocity.y - a->position.y * a->velocity.z + a->position.z * a->velocity.y},
            {e->velocity.z - a->velocity.z, a->velocity.y - e->velocity.y, a->position.z - e->position.z, e->position.y - a->position.y, e->position.y * e->velocity.z - e->position.z * e->velocity.y - a->position.y * a->velocity.z + a->position.z * a->velocity.y},
        }
    };

    if (!gauss_eliminate(&m_xy))
    {
        printf("WTF?!");
        exit(1);
    }

    if (!gauss_eliminate(&m_xz))
    {
        printf("WTF?!");
        exit(1);
    }

    if (!gauss_eliminate(&m_yz))
    {
        printf("WTF?!");
        exit(1);
    }

    #ifdef DEBUG_PRINT
    printf("X %Lf %Lf\n", m_xy.matrix[0][4], m_xz.matrix[0][4]);
    printf("Y %Lf %Lf\n", m_xy.matrix[1][4], m_yz.matrix[0][4]);
    printf("Z %Lf %Lf\n", m_yz.matrix[1][4], m_yz.matrix[1][4]);
    #endif

    long xr0 = round((m_xy.matrix[0][4] + m_xz.matrix[0][4])/2);
    long yr0 = round((m_xy.matrix[1][4] + m_yz.matrix[0][4])/2);
    long zr0 = round((m_yz.matrix[1][4] + m_xz.matrix[1][4])/2);

    #ifdef DEBUG_PRINT
    printf("%ld %ld %ld\n", xr0, yr0, zr0);
    #endif

    return xr0 + yr0 + zr0;
}

static bool gauss_eliminate(struct matrix *m)
{
    int ncols = m->nrows + 1;

    for (int i = 0; i < m->nrows; ++i)
    {
        long double d = m->matrix[i][i];
        if (d == 0) return false;

        for (int c = i; c < ncols; ++c)
            m->matrix[i][c] /= d;

        for (int j = i + 1; j < m->nrows; ++j)
        {
            long double mul = m->matrix[j][i];
            for (int c = i; c < ncols; ++c)
                m->matrix[j][c] -= m->matrix[i][c] * mul;
        }
    }

    for (int i = m->nrows - 1; i > 0; --i)
    {
        for (int j = 0; j < i; ++j)
        {
            long double mul = m->matrix[j][i];
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
