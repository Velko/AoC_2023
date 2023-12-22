#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

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


static void parse_line(const char *line);

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

    for (int c = 0; c < ncubes; ++c)
    {
        printf("(%d, %d, %d)\t[%d, %d, %d]\n",
            cubes[c].start.x,
            cubes[c].start.y,
            cubes[c].start.z,
            cubes[c].end.x - cubes[c].start.x + 1,
            cubes[c].end.y - cubes[c].start.y + 1,
            cubes[c].end.z - cubes[c].start.z + 1
        );

    }

    int total = 0;
    printf("Result: %d\n", total);

    
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
    ++ncubes;
}