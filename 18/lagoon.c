#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#define BUFFER_SIZE     256

#define MAX_INSTRUCTIONS    700

struct instr
{
    char dir;
    int steps;
};

struct instr instructions[MAX_INSTRUCTIONS];
int ninstructions;

static void parse_line(const char *line);

int main(void)
{
    char line[BUFFER_SIZE];
    FILE *input = fopen("input.txt", "r");

    for (ninstructions = 0;; ++ninstructions)
    {
        if (fgets(line, BUFFER_SIZE, input) == NULL) break; 
        parse_line(line);
    }

    // int total = 0;
    printf("Result: %d\n", ninstructions);

    fclose(input);
    return 0;
}


static void parse_line(const char *line)
{
    sscanf(line, "%c %d", &instructions[ninstructions].dir, &instructions[ninstructions].steps);
    //printf("%c, %d\n", instructions[ninstructions].dir, instructions[ninstructions].steps);
}