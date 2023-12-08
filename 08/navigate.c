#include <stdio.h>
#include <string.h>


#define INSTRUCTION_LEN_MAX     300
#define STEP_MAP_MAX           1000

struct step
{
    char dest[4];
    char left[4];
    char right[4];
};

char instructions[INSTRUCTION_LEN_MAX];

struct step step_map[STEP_MAP_MAX];

static void read_map(FILE *input);
static struct step* lookup_item(const char *key);

int main(void)
{
    FILE *input = fopen("input.txt", "r");

    fgets(instructions, INSTRUCTION_LEN_MAX, input);
    int instr_len = strlen(instructions) - 1; // -1 because there's a newline at the end

    getc(input); // discard the empty line

    read_map(input);

    const char *location = "AAA";
    int nsteps;
    for (nsteps = 0; strcmp(location, "ZZZ") != 0; ++nsteps)
    {
        struct step *step = lookup_item(location);

        switch (instructions[nsteps % instr_len])
        {
            case 'L':
                location = step->left;
                //printf("L");
                break;
            case 'R':
                location = step->right;
                //printf("R");
                break;
            default:
                printf("WTF?\n");
                return 1;
        }
    }

    printf("Result: %d\n", nsteps);

    fclose(input);
    return 0;
}


static void read_map(FILE *input)
{
    int len = 0;
    char left[8], right[8];
    for (;;++len)
    {
        int r = fscanf(input, "%s = %s %s\n", step_map[len].dest, left, right);
        if (r != 3) break;
        strncpy(step_map[len].left, left + 1, 3); step_map[len].left[3] = 0;
        strncpy(step_map[len].right, right, 3); step_map[len].right[3] = 0;
        //printf("'%s' -> '%s' '%s'\n", step_map[len].dest, step_map[len].left, step_map[len].right);
    }
    step_map[len].dest[0] = 0;
}

static struct step* lookup_item(const char *key)
{
    for (int i = 0; step_map[i].dest[0]; ++i)
    {
        if (strcmp(step_map[i].dest, key) == 0)
            return step_map + i;
    }

    return NULL;
}
