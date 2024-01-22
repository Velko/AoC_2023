#include <stdio.h>
#include <string.h>
#include <assert.h>


#define INSTRUCTION_LEN_MAX     300

#define LOCATIONS_MAX           1000
#define NPATHS                   6
#define MAP_LINE_LEN            20

char instructions[INSTRUCTION_LEN_MAX];
int step_map[2][LOCATIONS_MAX];

int locations[NPATHS];


char location_map[LOCATIONS_MAX][4];

static void read_map(FILE *input);
static int location_idx(const char* location, int len);

int main(void)
{
    FILE *input = fopen("input.txt", "r");

    fgets(instructions, INSTRUCTION_LEN_MAX, input);
    int instr_len = strlen(instructions) - 1; // -1 because there's a newline at the end

    for (int i = 0; i < instr_len; ++i)
    {
        switch (instructions[i])
        {
        case 'L':
            instructions[i] = 0;
            break;
        case 'R':
            instructions[i] = 1;
            break;
        default:
            printf("WTF?\n");
            return 1;
        }
    }

    getc(input); // discard the empty line

    read_map(input);

    fclose(input);

    for (int i = 0 ; i < NPATHS; ++i)
    {
        locations[i] = i + NPATHS;
    }

    long nsteps = 0;
    int paths_match = -1;  // any non-null to enter the loop
    while (paths_match) {
        for (int s = 0; s < instr_len && paths_match; ++s)
        {
            int *map = step_map[(int)instructions[s]];
            paths_match = 4; //NPATHS;  // reduce value for small-scale performance test
            for (int p = 0; p < NPATHS; ++p)
            {
                locations[p] = map[locations[p]];
                paths_match -= locations[p] < NPATHS;
            }
            ++nsteps;
        }
    }

    // part1:  18157
    printf("Result: %ld\n", nsteps);

    return 0;
}


static void strncpyz(char *dest, char *src, int n)
{
    strncpy(dest, src, n);
    dest[n] = 0;
}

static void read_map(FILE *input)
{
    char line[MAP_LINE_LEN];

    long map_pos = ftell(input);


    /* Use hardcoded number of paths. The indices for all ??Z locations will be < 6, all ??A are >=6 but < 12,
       the rest follows after that.
     */
    int n_idx = NPATHS * 2;
    int z_idx = 0;
    int a_idx = NPATHS;

    for (;;)
    {
        if (fgets(line, MAP_LINE_LEN, input) == NULL) break;

        switch(line[2])
        {
            case 'Z':
                strncpyz(location_map[z_idx++], line, 3);
                break;
            case 'A':
                strncpyz(location_map[a_idx++], line, 3);
                break;
            default:
                strncpyz(location_map[n_idx++], line, 3);
                break;
        }
    }

    assert(z_idx == NPATHS);
    assert(a_idx == NPATHS * 2);

    fseek(input, map_pos, SEEK_SET);

    char dest[8], left[8], right[8];
    for (;;)
    {
        int r = fscanf(input, "%s = %s %s\n", dest, left, right);
        if (r != 3) break;

        int idx = location_idx(dest, n_idx);

        step_map[0][idx] = location_idx(left + 1, n_idx);
        step_map[1][idx] = location_idx(right, n_idx);
    }
}

static int location_idx(const char* location, int nlen)
{
    for (int i = 0; i < nlen; ++i)
    {
        if (strncmp(location_map[i], location, 3) == 0)
            return i;
    }

    printf("WTF map lookup\n");
    assert(0);
}

// Run time for 5 / 6 = 10m6s = 606 s
// Result: 195887175797
// Calculation speed:
//  195887175797 / 606 = 323,246,164  steps/sec

// Estimate for 6 /6:
//   Result has to be 14,299,763,833,181
//   14299763833181 / 195887175797 == 73
//   606s * 73 == 44238 s
//   44238 s / 3600 s/h  = 12.29 h = 12h17m

