#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//#define DEBUG_PRINT

#define BUFFER_SIZE     24 * 1024       // the input is one huge string

char line[BUFFER_SIZE];

struct lens
{
    char label[8];
    int focal;
};


#define NUM_BOXES       256
#define BOX_SIZE        100

struct lens boxes[NUM_BOXES][BOX_SIZE];


static int calc_hash(const char *str);
static char parse_lens(char *str, struct lens *lens);
static void upsert_lens(struct lens *box, struct lens *new_lens);
static void remove_lens(struct lens *box, const char *label);
static long calculate_power();

#ifdef DEBUG_PRINT
static void print_boxes();
#endif

int main(void)
{
    FILE *input = fopen("input.txt", "r");
    fgets(line, BUFFER_SIZE, input);
    fclose(input);

    memset(boxes, 0, sizeof(boxes));

    char *savep;
    char *token = strtok_r(line, ",\n", &savep);
    while (token)
    {
        //printf("\nAfter \"%s\"\n", token);
        struct lens lens;
        char op = parse_lens(token, &lens);
        int box = calc_hash(lens.label);

        //printf("%c [%d] %s %d\n", op, box, lens.label, lens.focal);

        switch (op)
        {
        case '-':
            remove_lens(boxes[box], lens.label);
            break;
        case '=':
            upsert_lens(boxes[box], &lens);
            break;
        default:
            printf("Invalid op\n");
            exit(1);
            break;
        }

        #ifdef DEBUG_PRINT
        print_boxes();
        #endif

        token = strtok_r(NULL, ",\n", &savep);
    }


    long power = calculate_power();

    // result p1: 516070
    // result p2: 244981
    printf("Result: %ld\n", power);


    return 0;
}


static int calc_hash(const char *str)
{
    unsigned char hash = 0;

    for (const char *c = str; *c; ++c)
    {
        hash = (hash + *c) * 17;
    }
    //printf("%s %d\n", str, hash);

    return hash;
}

static char parse_lens(char *str, struct lens *lens)
{
    char *minus = strchr(str, '-');

    if (minus)
    {
        *minus = 0;
        if (strlen(str) > 7) { printf("too big\n"); exit(1); }
        strcpy(lens->label, str);
        lens->focal = 0;
        return '-';
    }

    char *savep;
    char *label = strtok_r(str, "=", &savep);
    if (strlen(label) > 7) { printf("too big\n"); exit(1); }
    strcpy(lens->label, label);

    char *focal = strtok_r(NULL, "=", &savep);
    lens->focal = atoi(focal);

    return '=';
}


static void upsert_lens(struct lens *box, struct lens *new_lens)
{
    int i;
    for (i = 0; box[i].focal; ++i)
    {
        if (strcmp(box[i].label, new_lens->label) == 0)
        {
            box[i].focal = new_lens->focal;
            return;
        }
    }
    memcpy(box + i, new_lens, sizeof(struct lens));
}

static void remove_lens(struct lens *box, const char *label)
{
    for (int i = 0; box[i].focal; ++i)
    {
        if (strcmp(box[i].label, label) == 0)
        {
            // 012345678
            //     |
            // 01235678
            memmove(box + i, box + i + 1, sizeof(struct lens) * (BOX_SIZE - i - 1));
        }
    }
}

#ifdef DEBUG_PRINT
static void print_boxes()
{
    for (int b = 0; b < 256; ++b)
    {
        if (boxes[b][0].focal)
        {
            printf("Box %d: ", b);

            for (int l = 0; boxes[b][l].focal; ++l)
                printf("[%s %d] ", boxes[b][l].label, boxes[b][l].focal);
            printf("\n");
        }
    }
}
#endif

static long calculate_power()
{
    long total_power = 0;

    for (long b = 0; b < 256; ++b)
    {
        if (boxes[b][0].focal)
        {
            for (int s = 0; boxes[b][s].focal; ++s)
            {
                long lens_power = (b + 1) * (s + 1) * boxes[b][s].focal;
                //printf("%s %ld\n", boxes[b][s].label, lens_power);
                total_power += lens_power;
            }
        }
    }

    return total_power;
}