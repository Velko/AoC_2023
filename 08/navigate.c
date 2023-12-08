#include <stdio.h>
#include <stdbool.h>
#include <string.h>


#define INSTRUCTION_LEN_MAX     300
#define STEP_MAP_RANGE          (32 * 32 * 32)

#define NPATHS_MAX              10

struct step
{
    int left;
    int right;
};

char instructions[INSTRUCTION_LEN_MAX];

struct step step_map[STEP_MAP_RANGE];

int locations[NPATHS_MAX];

static void read_map(FILE *input);
static struct step* lookup_item(int key);
static int encode_location(const char* location);
static long solve_location(int instr_len, int start);
static void print_location(int location);

int main(void)
{
    FILE *input = fopen("input.txt", "r");

    fgets(instructions, INSTRUCTION_LEN_MAX, input);
    int instr_len = strlen(instructions) - 1; // -1 because there's a newline at the end

    getc(input); // discard the empty line

    read_map(input);

    fclose(input);

    int npaths = 0;
    for (int i = 0 ; i < STEP_MAP_RANGE; i+=32)
    {
        if (step_map[i].left != -1)
        {
            locations[npaths++] = i;
        }
    }

    for (int i = 0; i < npaths; ++i)
    {
        long num = solve_location(instr_len, locations[i]);
        printf("%ld\n", num);
    }


    // part1:  18157
    //printf("Result: %d\n", nsteps);

    return 0;
}


static long solve_location(int instr_len, int start)
{
    long nsteps = 0;
    int location = start;
    for (;;) {
        for (int s = 0; s < instr_len; ++s)
        {
            struct step *step = lookup_item(location);

            switch (instructions[s])
            {
                case 'L':
                    location = step->left;
                    break;
                case 'R':
                    location = step->right;
                    break;
                default:
                    printf("WTF?\n");
                    return 1;
            }

            ++nsteps;
            if ((location & 31) == ('Z' - 'A'))
                return nsteps;
        }
    }
}



static void read_map(FILE *input)
{
    memset(step_map, -1, sizeof(step_map));
    char dest[8], left[8], right[8];
    for (;;)
    {
        int r = fscanf(input, "%s = %s %s\n", dest, left, right);
        if (r != 3) break;

        int idx = encode_location(dest);
        step_map[idx].left = encode_location(left + 1);
        step_map[idx].right = encode_location(right);
    }
}

static int encode_location(const char* location)
{
    return ((location[0] - 'A') << 10) | ((location[1] - 'A') << 5) | (location[2] - 'A');
}

static void print_location(int location)
{
    printf("%c%c%c\n", (location >> 10) + 'A', ((location >> 5) & 31) + 'A', (location &31) + 'A');
}

static struct step* lookup_item(int key)
{
    if (step_map[key].left != -1)
        return step_map + key;
    return NULL;
}
