#include <stdio.h>


#define BUFFER_SIZE     256



int main(void)
{
    char line[BUFFER_SIZE];
    FILE *input = fopen("input.txt", "r");

    for (;;)
    {
        if (fgets(line, BUFFER_SIZE, input) == NULL) break; 
        //TODO: process line
    }

    fclose(input);
    return 0;
}
