#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#define BUFFER_SIZE     150 /* input appears to be 140x140, add + extra place for \n and \0 */
#define MAX_GALAXIES    500 /* based on input */

#define EXPANSION_SCALE   2

struct coord
{
    long row;
    long col;
};


char space[BUFFER_SIZE][BUFFER_SIZE];
struct coord galaxies[MAX_GALAXIES];

long column_offsets[BUFFER_SIZE];
long row_offsets[BUFFER_SIZE];


int num_rows = 0;
int num_cols = 0;
int num_galaxies = 0;

static void expand_columns();
static void expand_rows();
static void load_galaxies();

int main(void)
{
    FILE *input = fopen("input.txt", "r");
    for (;; ++num_rows)
    {
        if (fgets(space[num_rows], BUFFER_SIZE, input) == NULL) break;
        if (!num_cols)
            num_cols = strchr(space[num_rows], '\n') - space[num_rows];
        space[num_rows][num_cols] = 0;
    }
    fclose(input);

    //printf("%d, %d\n", num_rows, num_cols);
    expand_columns();
    expand_rows();
    load_galaxies();

    // for (int row = 0; row < num_rows; ++row)
    // {
    //     printf("%s\n", space[row]);
    // }

    long total_distance = 0;

    for (int first = 0; first < num_galaxies; ++first)
    {
        for (int second = first + 1; second < num_galaxies; ++second)
        {
            int distance = labs(galaxies[first].row - galaxies[second].row)
                        + labs(galaxies[first].col - galaxies[second].col);
            total_distance += distance;
        }
    }

    // expected p1: 9639160
    printf("Result 1: %ld\n", total_distance);

    return 0;
}


static void expand_columns()
{
    long col_offset = 0;
    for (int col = 0; col < num_cols; ++col)
    {
        column_offsets[col] = col + col_offset;

        bool only_empty = true;
        for (int row = 0; row < num_rows; ++row)
        {
            only_empty &= space[row][col] == '.';
        }

        if (only_empty)
            col_offset += EXPANSION_SCALE - 1;
    }
}

static void expand_rows()
{
    long row_offset = 0;
    for (int row = 0; row < num_rows; ++row)
    {
        row_offsets[row] = row + row_offset;

        bool only_empty = true;
        for (int col = 0; col < num_cols; ++col)
        {
            only_empty &= space[row][col] == '.';
        }

        if (only_empty)
            row_offset += EXPANSION_SCALE - 1;
    }
}

static void load_galaxies()
{
    for (int row = 0; row < num_rows; ++row)
    {
        for (int col = 0; col < num_cols; ++col)
        {
            if (space[row][col] == '#')
            {
                galaxies[num_galaxies].row = row_offsets[row];
                galaxies[num_galaxies].col = column_offsets[col];
                ++num_galaxies;
            }
        }
    }

    printf("Num galaxies: %d\n", num_galaxies);
}