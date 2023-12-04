#include <stdio.h>
#include <ctype.h>

#define BUFFER_SIZE     256


static int parse_line(const char *line);

int main(void)
{
    FILE *input = fopen("input.txt", "r");
    char line[BUFFER_SIZE];

    int total = 0;
    for (;;)
    {
        if (fgets(line, BUFFER_SIZE, input) == NULL) break; 
        total += parse_line(line);
    }

    printf("%d\n", total);

    fclose(input);
    return 0;
}


static int parse_line(const char *line)
{
    char first = 0, last = 0;
    for (const char *p = line; *p; ++p)
    {
        if (isdigit(*p))
        {
            if (!first) first = *p;
            last = *p;
        }
    }

    return (first - '0') * 10 + (last - '0');
}