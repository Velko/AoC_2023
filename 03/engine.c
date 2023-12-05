#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>


#define BUFFER_SIZE     150 /* input appears to be 140x140, but let's add a bit extra for \n and \0 */

char lines[BUFFER_SIZE][BUFFER_SIZE];

static int parse_num(int col, int row);

int main(void)
{
    FILE *input = fopen("input.txt", "r");

    int num_rows = 0;

    for (;; ++num_rows)
    {
        if (fgets(lines[num_rows], BUFFER_SIZE, input) == NULL) break; 
    }

    int total = 0;

    for (int row = 0; row < num_rows; ++row)
    {
        for (const char *c = lines[row]; *c; ++c)
        {
            if (*c != '.' && *c != '\n' && !isdigit(*c))
            {
                int col = c - lines[row];
                printf("%c", *c);

                int num = parse_num(col, row - 1);
                total += num;
                if (num == 0)
                {
                    total += parse_num(col - 1, row - 1);
                    total += parse_num(col + 1, row - 1);
                }

                total += parse_num(col - 1, row);
                total += parse_num(col + 1, row);

                num = parse_num(col, row + 1);
                total += num;
                if (num == 0)
                {
                    total += parse_num(col - 1, row + 1);
                    total += parse_num(col + 1, row + 1);
                }
            }
        }
        printf("\n");
    }

    printf("%d\n", total);

    fclose(input);
    return 0;
}


static int parse_num(int col, int row)
{
    char digits[16];

    char *p = lines[row] + col;

    if (!isdigit(*p)) return 0;

    char *sp = p;
    for (; isdigit(*sp) && sp >= lines[row]; --sp);

    char *ep = p;
    for (; ep && isdigit(*ep); ++ep);

    int len = ep - sp - 1;

    strncpy(digits, sp + 1, len);
    digits[len] = 0;

    printf(" %s ", digits);

    return atoi(digits);
}