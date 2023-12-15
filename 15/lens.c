#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE     24 * 1024       // the input is one huge string

char line[BUFFER_SIZE];

static int calc_hash(const char *str);

int main(void)
{
    FILE *input = fopen("input.txt", "r");
    fgets(line, BUFFER_SIZE, input);
    fclose(input);

    int total = 0;

    char *savep;
    char *token = strtok_r(line, ",\n", &savep);
    while (token)
    {
        total += calc_hash(token);

        token = strtok_r(NULL, ",\n", &savep);
    }


    // result p1: 516070
    printf("Result: %d\n", total);


    return 0;
}


static int calc_hash(const char *str)
{
    unsigned char hash = 0;

    for (const char *c = str; *c; ++c)
    {
        hash = (hash + *c) * 17;
    }
    printf("%s %d\n", str, hash);

    return hash;
}