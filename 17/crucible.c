#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define BUFFER_SIZE     150

int main(void)
{
    char line[BUFFER_SIZE];
    FILE *input = fopen("input.txt", "r");


    for (;;)
    {
        if (fgets(line, BUFFER_SIZE, input) == NULL) break; 
    }
    fclose(input);

    int total = 0;
    printf("Result: %d\n", total);

    return 0;
}
