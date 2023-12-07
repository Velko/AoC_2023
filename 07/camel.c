#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE     256
#define MAX_HANDS       1024
#define NCARDS          13

#define FIVE        6
#define FOUR        5
#define FULL        4
#define THREE       3
#define TWOPAIR     2
#define PAIR        1
#define HIGH        0

const char *labels = "23456789TJQKA";


struct card_count
{
    int card;
    int count;
};

struct camel_hand
{
    char cards[6];
    int bid;
    int values[5];
    int ctype;
};


struct camel_hand hands[MAX_HANDS];

static void parse_line(const char *line, struct camel_hand *hand);
static void calculate_value(struct camel_hand *hand);
static int compare_hands(const void *a, const void *b);

int main(void)
{
    char line[BUFFER_SIZE];
    FILE *input = fopen("input.txt", "r");

    int n_hands = 0;
    for (;;)
    {
        if (fgets(line, BUFFER_SIZE, input) == NULL) break;
        parse_line(line, hands + n_hands);
        calculate_value(hands + n_hands);
        ++n_hands;
    }


    qsort(hands, n_hands, sizeof(struct camel_hand), compare_hands);

    int total = 0;

    for (int i = 0; i < n_hands; ++i)
    {
        total += hands[i].bid * (i + 1);
        printf("%d '%s'\n", hands[i].bid, hands[i].cards);
    }

    printf("Result: %d\n", total);

    fclose(input);
    return 0;
}


static void parse_line(const char *line, struct camel_hand *hand)
{
    sscanf(line, "%s %d", hand->cards, &hand->bid);
}

static int compare_counts_count(const void *a, const void *b);

static void calculate_value(struct camel_hand *hand)
{
    struct card_count counts[NCARDS];
    memset(counts, 0, sizeof(counts));

    for (int r = 0; r < 13; ++r)
    {
        counts[r].card = r;
        for (int i = 0; i < 5; ++i)
        {
            if (labels[r] == hand->cards[i])
            {
                ++counts[r].count;
                hand->values[i] = r + 2;
            }
        }
    }

    qsort(counts, NCARDS, sizeof(struct card_count), compare_counts_count);

    if (counts[0].count == 5)
        hand->ctype = FIVE;
    else if (counts[0].count == 4)
        hand->ctype = FOUR;
    else if (counts[0].count == 3 && counts[1].count == 2)
        hand->ctype = FULL;
    else if (counts[0].count == 3)
        hand->ctype = THREE;
    else if (counts[0].count == 2 && counts[1].count == 2)
        hand->ctype = TWOPAIR;
    else if (counts[0].count == 2)
        hand->ctype = PAIR;
    else
        hand->ctype = HIGH;

    // for (int r = 0; r < 5; ++r)
    // {
    //     printf("%c %d %d\n", labels[counts[r].card], hand->values[r], counts[r].count);
    // }
}

static int compare_counts_count(const void *a, const void *b)
{
    return ((struct card_count *)b)->count - ((struct card_count *)a)->count;
}

static int compare_hands(const void *a, const void *b)
{
    const struct camel_hand *handa = (const struct camel_hand *)a;
    const struct camel_hand *handb = (const struct camel_hand *)b;

    int cmp = handa->ctype - handb->ctype;

    if (cmp != 0) return cmp;

    for (int i = 0; i < 5; ++i)
    {
        cmp = handa->values[i] - handb->values[i];
        if (cmp != 0) return cmp;
    }

    printf("%s is equal to %s\n", handb->cards, handa->cards);
    return 0;
}