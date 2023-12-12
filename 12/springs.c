#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>


#define BUFFER_SIZE     256
#define MAX_GROUPS        8
#define TEMPLATE_LEN     25

static int process_line(char *line);
static int process_record(const char *template, unsigned *groups, int num_groups);
static bool check_counts(char *record, unsigned *groups, int num_groups);
static void self_tests();

int main(void)
{
    //self_tests();

    char line[BUFFER_SIZE];
    FILE *input = fopen("input.txt", "r");

    int total = 0;

    for (;;)
    {
        if (fgets(line, BUFFER_SIZE, input) == NULL) break;
        total += process_line(line);
    }

    // p1 result: 7191
    printf("Result: %d\n", total);

    fclose(input);
    return 0;
}


static int process_line(char *line)
{
    char *outersp, *innersp;
    char *template = strtok_r(line, " ", &outersp);

    char *groups_token = strtok_r(NULL, " ", &outersp);

    unsigned groups[MAX_GROUPS];
    int num_groups = 0;

    char *token = strtok_r(groups_token, ",", &innersp);
    while (token) {
        groups[num_groups++] = atoi(token);
        token = strtok_r(NULL, ",", &innersp);
    }

    //printf("Unk: %d, groups: %d tlen: %lu\n", num_unknown, num_groups, strlen(template));
    return process_record(template, groups, num_groups);
}

static int process_record(const char *template, unsigned *groups, int num_groups)
{
    char reconstructed[TEMPLATE_LEN];
    int num_unknown = 0;
    for (const char *s = template; *s; ++s)
        num_unknown += *s == '?';

    int possibilites = 0;

    int unk_limit = 1 << num_unknown;

    for (int unk = 0; unk < unk_limit; ++unk)
    {
        int i;
        int unk_bit_idx = 0;
        for (i = 0; template[i]; ++i)
        {
            if (template[i] == '?')
            {
                reconstructed[i] = (unk & (1 << unk_bit_idx)) ? '#' : '.';
                ++unk_bit_idx;
            }
            else
                reconstructed[i] = template[i];
        }
        reconstructed[i] = 0;

        if (check_counts(reconstructed, groups, num_groups))
            ++possibilites;
    }

    return possibilites;
}

static bool check_counts(char *record, unsigned *groups, int num_groups)
{
    char *savep;
    char *tok = strtok_r(record, ".", &savep);
    int c_group;
    for (c_group = 0; tok && c_group < num_groups; ++c_group )
    {
        if (groups[c_group] != strlen(tok))
            return false;

        tok = strtok_r(NULL, ".", &savep);
    }

    return tok == NULL && c_group == num_groups;
}

static void self_tests()
{
    char record[20];
    unsigned counts[]  = { 2, 1 };

    strcpy(record, "..##.#..");
    bool actual = check_counts(record, counts, 2);
    assert(actual == true);

    strcpy(record, "##...#");
    actual = check_counts(record, counts, 2);
    assert(actual == true);

    strcpy(record, "##...#.#");
    actual = check_counts(record, counts, 2);
    assert(actual == false);


    strcpy(record, "..##");
    actual = check_counts(record, counts, 2);
    assert(actual == false);

}