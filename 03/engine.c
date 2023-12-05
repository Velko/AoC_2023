#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>


#define BUFFER_SIZE     150 /* input appears to be 140x140, but let's add a bit extra for \n and \0 */

char lines[BUFFER_SIZE][BUFFER_SIZE];

static int parse_num(int col, int row);
static void count_mul_n(int *count, int *ratio, int num);


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
            if (*c == '*')
            {
                int col = c - lines[row];

                int num_count = 0;
                int ratio = 1;

                int num = parse_num(col, row - 1);
                count_mul_n(&num_count, &ratio, num);
                if (num == 0)
                {
                    count_mul_n(&num_count, &ratio, parse_num(col - 1, row - 1));
                    count_mul_n(&num_count, &ratio, parse_num(col + 1, row - 1));
                }

                count_mul_n(&num_count, &ratio, parse_num(col - 1, row));
                count_mul_n(&num_count, &ratio, parse_num(col + 1, row));

                num = parse_num(col, row + 1);
                count_mul_n(&num_count, &ratio, num);
                if (num == 0)
                {
                    count_mul_n(&num_count, &ratio, parse_num(col - 1, row + 1));
                    count_mul_n(&num_count, &ratio, parse_num(col + 1, row + 1));
                }

                if (num_count == 2)
                {
                    printf("*%d*\n", ratio);
                    total += ratio;
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


static void count_mul_n(int *count, int *ratio, int num)
{
    if (num != 0)
    {
        ++(*count);
        (*ratio) *= num;
    }
}