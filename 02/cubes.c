#include <stdio.h>
#include <string.h>


#define BUFFER_SIZE     256

static int parse_line(char *line);

int main(void)
{
    char line[BUFFER_SIZE];
    FILE *input = fopen("input.txt", "r");

    int total = 0;

    for (;;)
    {
        if (fgets(line, BUFFER_SIZE, input) == NULL) break; 
        total += parse_line(line);
        //break;
    }

    printf("Result: %d\n", total);

    fclose(input);
    return 0;
}

static int parse_line(char *line)
{
    char *outer_savep, *inner_savep;
    char *game_token = strtok_r(line, ":", &outer_savep);

    int game_id;
    sscanf(game_token, "%*s %d", &game_id);

    int max_red = 0, max_green = 0, max_blue = 0;

    while (game_token = strtok_r(NULL, ";", &outer_savep))
    {
        char *pull_token = strtok_r(game_token, ",", &inner_savep);
        while (pull_token)
        {
            int count; char color;
            sscanf(pull_token, "%d %c", &count, &color);

            switch (color)
            {
            case 'r':
                if (max_red < count) max_red = count;
                break;
            case 'g':
                if (max_green < count) max_green = count;
                break;
            case 'b':
                if (max_blue < count) max_blue = count;
                break;
            }
            pull_token = strtok_r(NULL, ",", &inner_savep);
        }
    }

    if (max_red <= 12 && max_green <= 13 && max_blue <= 14)
        return game_id;

    printf("%d\n", game_id);
    printf("red: %d, green: %d, blue: %d\n", max_red, max_green, max_blue);

    return 0;
}