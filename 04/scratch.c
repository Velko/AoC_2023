#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define BUFFER_SIZE     256
#define N_WINNING        10
#define N_GUESSES        25
#define N_CARDS         214

struct card_copies
{
    int score;
    int copies;
};

struct card_copies score_table[N_CARDS];

static int parse_line(char *line);

int main(void)
{
    char line[BUFFER_SIZE];
    FILE *input = fopen("input.txt", "r");

    int n_cards = 0;

    for (;; ++n_cards)
    {
        if (fgets(line, BUFFER_SIZE, input) == NULL) break; 
        score_table[n_cards].score = parse_line(line);
        score_table[n_cards].copies = 1;
    }

    int total = 0;
    for (int i = 0; i < n_cards; ++i)
    {
        total += score_table[i].copies;
        printf("%d\n", score_table[i].copies);
        for (int c = 0; c < score_table[i].score; ++c)
            score_table[i + c + 1].copies += score_table[i].copies;
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

    int card_score = 0;

    for (int g = 0; g < n_guessed; ++g)
    {
        for (int w = 0; w < n_winning; ++w)
        {
            if (guessed_numbers[g] == winning_numbers[w])
                ++card_score;
        }
    }

    printf("  %d\n", card_score);

    return card_score;
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