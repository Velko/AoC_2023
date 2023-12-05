#include <stdio.h>
#include <ctype.h>


#define BUFFER_SIZE     150 /* input appears to be 140x140, but let's add a bit extra for \n and \0 */

char lines[BUFFER_SIZE][BUFFER_SIZE];

static int parse_line(const char *line);

int main(void)
{
    FILE *input = fopen("input.txt", "r");

    int num_rows = 0;

    for (;; ++num_rows)
    {
        if (fgets(lines[num_rows], BUFFER_SIZE, input) == NULL) break; 
    }

    for (int row = 0; row < num_rows; ++row)
    {
        for (const char *c = lines[row]; *c; ++c)
        {
            if (*c != '.' && *c != '\n' && !isdigit(*c))
            {
                int col = c - lines[row];
                printf("(%d, %d)\t%c\n", col, row, *c);
            }
        }
    }

    fclose(input);
    return 0;
}


static int parse_line(const char *line)
{
    return 0;
}