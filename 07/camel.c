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

const char *labels = "J23456789TQKA";


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
static int calculate_ctype(const char *cards);
static int calculate_ctype_with_jokers(const char *cards);
static void calculate_values(struct camel_hand *hand);
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
        calculate_values(hands + n_hands);
        hands[n_hands].ctype = calculate_ctype_with_jokers(hands[n_hands].cards);
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

static void calculate_values(struct camel_hand *hand)
{
    for (int r = 0; r < 13; ++r)
    {
        for (int i = 0; i < 5; ++i)
        {
            if (labels[r] == hand->cards[i])
            {
                hand->values[i] = r + 1;
            }
        }
    }
}

static int calculate_ctype_with_jokers(const char *cards)
{
    int njokers = 0;
    for (int i = 0; i < 5; ++i)
    {
        if (cards[i] == 'J') ++njokers;
    }

    /* no jokers or all jokers -> work as usual */
    if (njokers == 0 || njokers == 5)
        return calculate_ctype(cards);

    int ctype = HIGH;
    for (int n = 0; n < 5; ++n)
    {
        if (cards[n] != 'J')
        {
            char replacement = cards[n];
            char replaced[6];
            strcpy(replaced, cards);

            for (int j = 0; j < 5; ++j)
            {
                if (replaced[j] == 'J')
                    replaced[j] = replacement;
            }

            int new_ctype = calculate_ctype(replaced);
            if (new_ctype > ctype) ctype = new_ctype;
        }
    }

    return ctype;
}

static int compare_counts_count(const void *a, const void *b);

static int calculate_ctype(const char *cards)
{
    struct card_count counts[NCARDS];
    memset(counts, 0, sizeof(counts));

    for (int r = 0; r < 13; ++r)
    {
        counts[r].card = r;
        for (int i = 0; i < 5; ++i)
        {
            if (labels[r] == cards[i])
            {
                ++counts[r].count;
            }
        }
    }

    qsort(counts, NCARDS, sizeof(struct card_count), compare_counts_count);

    if (counts[0].count == 5)
        return FIVE;
    else if (counts[0].count == 4)
        return FOUR;
    else if (counts[0].count == 3 && counts[1].count == 2)
        return FULL;
    else if (counts[0].count == 3)
        return THREE;
    else if (counts[0].count == 2 && counts[1].count == 2)
        return TWOPAIR;
    else if (counts[0].count == 2)
        return PAIR;
    else
        return HIGH;
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