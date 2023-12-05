#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define BUFFER_SIZE     256
#define N_WINNING        10
#define N_GUESSES        25


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
    }

    printf("Result: %d\n", total);

    fclose(input);
    return 0;
}


static int parse_numbers(char *row, int *results);

static int parse_line(char *line)
{
    char *outer_savep;
    char *card_token = strtok_r(line, ":", &outer_savep);


    int card_id;
    sscanf(card_token, "%*s %d", &card_id);

    printf("C %d", card_id);

    card_token = strtok_r(NULL, "|", &outer_savep);

    int winning_numbers[N_WINNING];
    int n_winning = parse_numbers(card_token, winning_numbers);


    card_token = strtok_r(NULL, "|", &outer_savep);
    int guessed_numbers[N_GUESSES];
    int n_guessed = parse_numbers(card_token, guessed_numbers);

    int card_score = 1;

    for (int g = 0; g < n_guessed; ++g)
    {
        for (int w = 0; w < n_winning; ++w)
        {
            if (guessed_numbers[g] == winning_numbers[w])
                card_score <<= 1;
        }
    }

    printf("  %d\n", card_score >> 1);

    return card_score >> 1;
}

static int parse_numbers(char *row, int *results)
{
    char *inner_savep;
    int count = 0;

    char *num_token = strtok_r(row, " ", &inner_savep);
    while (num_token)
    {
        results[count++] = atoi(num_token);
        num_token = strtok_r(NULL, " ", &inner_savep);
    }

    return count;

//    printf ("%d\n", count);
}