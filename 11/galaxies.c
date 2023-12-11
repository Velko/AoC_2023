#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#define BUFFER_SIZE     300 /* input appears to be 140x140, but it could expand twice + extra place for \n and \0 */
#define MAX_GALAXIES    500 /* based on input */

struct coord
{
    int row;
    int col;
};



char space[BUFFER_SIZE][BUFFER_SIZE];
struct coord galaxies[MAX_GALAXIES];

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
            int distance = abs(galaxies[first].row - galaxies[second].row)
                        + abs(galaxies[first].col - galaxies[second].col);
            total_distance += distance;
        }
    }

    // expected p1: 9639160
    printf("Result 1: %ld\n", total_distance);

    return 0;
}


static void expand_columns()
{
    for (int col = 0; col < num_cols; ++col)
    {
        bool only_empty = true;
        for (int row = 0; row < num_rows; ++row)
        {
            only_empty &= space[row][col] == '.';
        }

        if (only_empty)
        {
            //printf("Empty col: %d\n", col);
            for (int row = 0; row < num_rows; ++row)
            {
                memmove(space[row]+col+1, space[row]+col, num_cols - col + 1);
            }
            ++num_cols;
            ++col;
        }
    }
}

static void expand_rows()
{
    for (int row = 0; row < num_rows; ++row)
    {
        bool only_empty = true;
        for (int col = 0; col < num_cols; ++col)
        {
            only_empty &= space[row][col] == '.';
        }

        if (only_empty)
        {
            //printf("Empty row: %d\n", row);
            memmove(space + row + 1, space + row, (num_rows - row) * BUFFER_SIZE);
            ++num_rows;
            ++row;
        }
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
                galaxies[num_galaxies].row = row;
                galaxies[num_galaxies].col = col;
                ++num_galaxies;
            }
        }
    }

    printf("Num galaxies: %d\n", num_galaxies);
}