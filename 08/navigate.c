#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define INSTRUCTION_LEN_MAX     300

/* For top performance, locations are re-packed into integers as tight as possible:
   There are 26 letters in alphabet, they can be packed into 5-bits, 32 distinct values.
   All values from 'AAA' to 'ZZZ' then ranges (with some gaps) from 0 to 26624 a reasonable
   value for array index. Accessing item in array by index is much faster than looking it up
   in dictionary or list.

   In the end this performance optimization does not matter much now, but my first attempt
   was to brute-force the problem, and I kinda like the efficiency.
*/

#define CHAR_BITS               5
#define CHAR_NUM                (1 << 5)
#define CHAR_MASK               (CHAR_NUM - 1)
#define STEP_MAP_RANGE          (CHAR_NUM * CHAR_NUM * CHAR_NUM)
#define CHAR_IDX(C)             ((C) - 'A')

#define NPATHS_MAX              10

struct step
{
    int left;
    int right;
};

char instructions[INSTRUCTION_LEN_MAX];
struct step step_map[STEP_MAP_RANGE];

static void read_map(FILE *input);
static int encode_location(const char* location);
static long solve_location(int instr_len, int start);
//static void print_location(int location);
static long lcm(long a, long b);
static long gcd(long a, long b);

int main(void)
{
    FILE *input = fopen("input.txt", "r");

    fgets(instructions, INSTRUCTION_LEN_MAX, input);
    int instr_len = strlen(instructions) - 1; // -1 because there's a newline at the end

    getc(input); // discard the empty line

    read_map(input);

    fclose(input);

    long total_steps = 1;

    for (int i = 0 ; i < STEP_MAP_RANGE; i+=CHAR_NUM)
    {
        if (step_map[i].left != -1)
        {
            long num_steps = solve_location(instr_len, i);
            total_steps = lcm(total_steps, num_steps);
        }
    }

    // part1: 18157
    // part2: 14299763833181
    printf("Result: %ld\n", total_steps);

    return 0;
}

static long solve_location(int instr_len, int start)
{
    long nsteps = 0;
    int location = start;
    for (;;) {
        for (int s = 0; s < instr_len; ++s)
        {
            switch (instructions[s])
            {
                case 'L':
                    location = step_map[location].left;
                    break;
                case 'R':
                    location = step_map[location].right;
                    break;
                default:
                    printf("WTF?\n");
                    exit(1);
            }

            ++nsteps;
            if ((location & CHAR_MASK) == CHAR_IDX('Z'))
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
    return (CHAR_IDX(location[0]) << CHAR_BITS * 2)
         | (CHAR_IDX(location[1]) << CHAR_BITS * 1)
         | (CHAR_IDX(location[2]) << CHAR_BITS * 0);
}

// static void print_location(int location)
// {
//     printf("%c%c%c\n", 
//         ((location >> CHAR_BITS * 2) & CHAR_MASK) + 'A',
//         ((location >> CHAR_BITS * 1) & CHAR_MASK) + 'A',
//         ((location >> CHAR_BITS * 0) & CHAR_MASK) + 'A');
// }

static long lcm(long a, long b)
{
    return (a / gcd(a, b)) * b;
}

static long gcd(long a, long b)
{
    while (a > 0 && b > 0)
    {
        if (a > b)
            a = a % b;
        else
            b = b % a;
    }

    if (a == 0)
        return b;
    return a;
}