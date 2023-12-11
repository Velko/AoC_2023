#include <stdio.h>
#include <string.h>


#define BUFFER_SIZE     300 /* input appears to be 140x140, but it could expand twice + extra place for \n and \0 */

char space[BUFFER_SIZE][BUFFER_SIZE];

int main(void)
{
    FILE *input = fopen("input.txt", "r");
    int num_rows = 0;
    int num_cols = 0;
    for (;; ++num_rows)
    {
        if (fgets(space[num_rows], BUFFER_SIZE, input) == NULL) break;
        if (!num_cols)
            num_cols = strchr(space[num_rows], '\n') - space[num_rows];
        space[num_rows][num_cols] = 0;
    }
    fclose(input);

    //printf("%d, %d\n", num_rows, num_cols);

    for (int row = 0; row < num_rows; ++row)
    {
        printf("%s\n", space[row]);
    }

    return 0;
}
