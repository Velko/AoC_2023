#include <stdio.h>
#include <string.h>
#include <stdbool.h>


#define MIRROR_SIZE     25

char mirror[MIRROR_SIZE][MIRROR_SIZE];

static int check_mirror(FILE *input);

int main(void)
{
    FILE *input = fopen("input.txt", "r");

    int total = 0;

    while (!feof(input))
    {
        total = check_mirror(input);
    }

    printf("Result: %d\n", total);

    fclose(input);
    return 0;
}

static int check_reflection_horizontal(int nlines);


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

    int row = check_reflection_horizontal(nlines);

    return row;
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