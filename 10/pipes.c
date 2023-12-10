#include <stdio.h>
#include <string.h>


#define BUFFER_SIZE     150 /* input appears to be 140x140, but let's add a bit extra for \n and \0 */

char maze[BUFFER_SIZE][BUFFER_SIZE];

int main(void)
{
    FILE *input = fopen("input.txt", "r");

    int num_rows = 0;
    int start_row = -1;
    int start_col = -1;

    for (;; ++num_rows)
    {
        if (fgets(maze[num_rows], BUFFER_SIZE, input) == NULL) break;
        char *s_ptr = strchr(maze[num_rows], 'S');
        if (s_ptr)
        {
            start_row = num_rows;
            start_col = s_ptr - maze[num_rows];
        }
    }


    printf("Rows: %d\n", num_rows);
    printf("Start: (%d, %d)\n", start_row, start_col);


    return 0;


    int total = 0;

    for (;;)
    {
    }

    printf("Result: %d\n", total);

    fclose(input);
    return 0;
}
