#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#define BUFFER_SIZE     256

static int parse_line(const char *line);

int main(void)
{
    char line[BUFFER_SIZE];
    FILE *input = fopen("input.txt", "r");

    int total = 0;

    for (;;)
    {
        if (fgets(line, BUFFER_SIZE, input) == NULL) break; 
        total += parse_line(line);
    }

    printf("Result: %d\n", total);

    fclose(input);
    return 0;
}


static int parse_line(const char *line)
{
    return 0;
}