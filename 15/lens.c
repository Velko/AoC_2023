#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE     24 * 1024       // the input is one huge string

char line[BUFFER_SIZE];

struct lens
{
    char label[8];
    int focal;
};

static int calc_hash(const char *str);
static char parse_lens(char *str, struct lens *lens);

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
        struct lens lens;
        char op = parse_lens(token, &lens);
        int box = calc_hash(lens.label);

        printf("%c [%d] %s %d\n", op, box, lens.label, lens.focal);

        //total += calc_hash(token);

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
    //printf("%s %d\n", str, hash);

    return hash;
}

static char parse_lens(char *str, struct lens *lens)
{
    char *minus = strchr(str, '-');

    if (minus)
    {
        *minus = 0;
        if (strlen(str) > 7) { printf("too big\n"); exit(1); }
        strcpy(lens->label, str);
        lens->focal = 0;
        return '-';
    }

    char *savep;
    char *label = strtok_r(str, "=", &savep);
    if (strlen(label) > 7) { printf("too big\n"); exit(1); }
    strcpy(lens->label, label);

    char *focal = strtok_r(NULL, "=", &savep);
    lens->focal = atoi(focal);

    return '=';
}
