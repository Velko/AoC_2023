#include <stdio.h>
#include <string.h>


#define INSTRUCTION_LEN_MAX     300
#define STEP_MAP_RANGE          (32 * 32 * 32)

struct step
{
    int left;
    int right;
};

char instructions[INSTRUCTION_LEN_MAX];

struct step step_map[STEP_MAP_RANGE];

static void read_map(FILE *input);
static struct step* lookup_item(int key);
static int encode_location(const char* location);

int main(void)
{
    FILE *input = fopen("input.txt", "r");

    fgets(instructions, INSTRUCTION_LEN_MAX, input);
    int instr_len = strlen(instructions) - 1; // -1 because there's a newline at the end

    getc(input); // discard the empty line

    read_map(input);


    int destination = encode_location("ZZZ");
    int location = encode_location("AAA");


    int nsteps;
    for (nsteps = 0; location != destination; ++nsteps)
    {
        struct step *step = lookup_item(location);

        switch (instructions[nsteps % instr_len])
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
    }

    // part1:  18157
    printf("Result: %d\n", nsteps);

    fclose(input);
    return 0;
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
    return ((location[0] - 'A' << 10)) | ((location[1] - 'A' << 5)) | ((location[2] - 'A'));
}

static struct step* lookup_item(int key)
{
    if (step_map[key].left != -1)
        return step_map + key;
    return NULL;
}
