#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>


#define MIRROR_SIZE     25

char mirror[MIRROR_SIZE][MIRROR_SIZE];

static int check_mirror(FILE *input);

int main(void)
{
    FILE *input = fopen("input.txt", "r");

    int total = 0;

    while (!feof(input))
    {
        total += check_mirror(input);
    }

    // Result p1: 34918
    printf("Result: %d\n", total);

    fclose(input);
    return 0;
}

static int check_reflection_horizontal(int nlines);
static int transpose(int nlines);
static int check_reflections(int nlines);

static int check_mirror(FILE *input)
{
    int nlines = 0;
    for (;;)
    {
        if (fgets(mirror[nlines], MIRROR_SIZE, input) == NULL) break;
        int len = strlen(mirror[nlines]);
        if (len > 1 && mirror[nlines][len-1] == '\n')
            mirror[nlines][len-1] = 0; // remove newline
        else
            break;
        ++nlines;
    }

    // for (int i = 0; i < nlines; ++i)
    //     printf("%s\n", mirror[i]);

    // printf("----------------------------------\n");



    int result = check_reflections(nlines);

    if (result != -1) return result;

    assert(!"Not found!");
    return -1;
}

static int check_reflections(int nlines)
{
    int row = check_reflection_horizontal(nlines);
    if (row != -1) return row * 100;

    nlines = transpose(nlines);
    int col = check_reflection_horizontal(nlines);

    return col;
}


static int check_reflection_horizontal(int nlines)
{
    for (int split_row = 1; split_row < nlines; ++split_row)
    {
        if (strcmp(mirror[split_row], mirror[split_row - 1]) == 0)
        {
            bool all_match = true;
            for (int dist = 1; dist + split_row < nlines && split_row - dist - 1 >= 0 && all_match; ++dist)
                all_match &= strcmp(mirror[split_row + dist], mirror[split_row - dist - 1]) == 0;

            if (all_match)
                return split_row;
        }
    }

    return -1;
}

static int transpose(int nlines)
{
    char source[MIRROR_SIZE][MIRROR_SIZE];
    memcpy(source, mirror, sizeof(mirror));

    int ncols = strlen(source[0]);

    for (int c = 0; c < ncols; ++c)
    {
        for (int r = 0; r < nlines; ++r)
            mirror[c][r] = source[r][c];
        mirror[c][nlines] = 0;
    }

    return ncols;
}