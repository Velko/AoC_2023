#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>


#define BUFFER_SIZE     256
#define MAX_GROUPS       40
#define TEMPLATE_LEN    128

static int process_line(char *line);
static int process_record(const char template[], int start_idx, unsigned groups[]);
static bool check_counts(char *record, unsigned *groups);
static void self_tests();

int main(void)
{
    self_tests();
    //return 0;

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

    printf("%s ", template);

    char *groups_token = strtok_r(NULL, " \n", &outersp);

    printf("%s ", groups_token);

    unsigned groups[MAX_GROUPS];
    int num_groups = 0;

    char *token = strtok_r(groups_token, ",", &innersp);
    while (token) {
        groups[num_groups++] = atoi(token);
        token = strtok_r(NULL, ",", &innersp);
    }
    groups[num_groups] = 0;

    int result = process_record(template, 0, groups);

    printf("%d\n", result);

    char *expected_p1_token = strtok_r(NULL, " \n", &outersp);
    if (expected_p1_token)
    {
        int expected = atoi(expected_p1_token);
        assert(expected == result);
    }

    //printf("Unk: %d, groups: %d tlen: %lu\n", num_unknown, num_groups, strlen(template));
    return result;
}

static bool fill_group(char *dest, const char *template, int grp);

static int process_record(const char template[], int start_idx, unsigned groups[])
{
    char reconstructed[TEMPLATE_LEN];

    unsigned grp = *groups;

    if (grp == 0)
        return 1;

    int sub_counts = 0;

    for (;;)
    {
        // scan forward until #, ? or \0
        for( ; template[start_idx] == '.'; ++start_idx);

        switch (template[start_idx])
        {
        case '#':
            /* arrived at beginning of known damage, must try to use all group */
            strcpy(reconstructed, template);
            if (fill_group(reconstructed + start_idx, template + start_idx, grp))
            {
                // good, go to next group
                int spacer = 0;
                if (template[start_idx + grp] == '?')
                {
                    reconstructed[start_idx + grp] = '.';
                    spacer = 1;
                }
                return process_record(reconstructed, start_idx + grp + spacer, groups + 1);
            }
            else
                return 0; // did not fit, stop trying
            break;
        case '?':
            /* arrived at the beginning of unknown, can try one by one */
            strcpy(reconstructed, template);
            for (; template[start_idx] == '?' || template[start_idx] == '#' ; ++start_idx)
            {
                strcpy(reconstructed + start_idx, template + start_idx);
                if (fill_group(reconstructed + start_idx, template + start_idx, grp))
                {
                    // good, try to go deeper
                    int spacer = 0;
                    if (template[start_idx + grp] == '?')
                    {
                        reconstructed[start_idx + grp] = '.';
                        spacer = 1;
                    }

                    sub_counts += process_record(reconstructed, start_idx + grp + spacer, groups + 1);
                }
                if (template[start_idx] == '?')
                    reconstructed[start_idx] = '.';
            }
            break;
        case 0:
            return sub_counts;
        default:
            assert("Unexpected char");
            break;
        }
    }
    assert("What?");
    return -1;
}

static bool fill_group(char *dest, const char *template, int grp)
{
    int p;
    for (p = 0; p < grp; ++p)
    {
        if (template[p] == '#' || template[p] == '?')
            dest[p] = '#';
        else
            /* did not fit */
            return false;
    }
    return template[p] != '#';
}

static int process_record_x(const char *template, unsigned *groups)
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

        if (check_counts(reconstructed, groups))
            ++possibilites;
    }

    return possibilites;
}

static bool check_counts(char *record, unsigned *groups)
{
    char *savep;
    char *tok = strtok_r(record, ".", &savep);
    int c_group;
    for (c_group = 0; tok && groups[c_group]; ++c_group )
    {
        if (groups[c_group] != strlen(tok))
            return false;

        tok = strtok_r(NULL, ".", &savep);
    }

    return tok == NULL && groups[c_group] == 0;
}

static void self_tests()
{
    char record[20];
    unsigned counts[]  = { 2, 1, 0 };

    strcpy(record, "..##.#..");
    bool actual = check_counts(record, counts);
    assert(actual == true);

    strcpy(record, "##...#");
    actual = check_counts(record, counts);
    assert(actual == true);

    strcpy(record, "##...#.#");
    actual = check_counts(record, counts);
    assert(actual == false);


    strcpy(record, "..##");
    actual = check_counts(record, counts);
    assert(actual == false);

    unsigned c1[] =  { 3, 0 };
    // not fit
    int num = process_record("..#?", 0, c1);
    assert(num == 0);

    // fit exactly
    num = process_record("..##?", 0, c1);
    assert(num == 1);

    // fit with trailing good
    num = process_record("..##?.", 0, c1);
    assert(num == 1);

    // fit and unknown reminder
    num = process_record("..##??", 0, c1);
    assert(num == 1);

    // unknown start trailing #, 1 option
    num = process_record("..??##", 0, c1);
    assert(num == 1);

    // can't fit, result too long
    num = process_record("..#??#", 0, c1);
    assert(num == 0);

    // need separator
    unsigned c2[] =  { 1, 1, 0 };
    num = process_record("???", 0, c2);
    assert(num == 1);

    // alternating
    // .#.###.#.######
    unsigned c3[] =  { 1, 3, 1, 6, 0 };
    num = process_record("?#?#?#?#?#?#?#?", 0, c3);
    assert(num == 1);

    // ????.######..#####. 1,6,5
    // ????.######..#####.
    unsigned c4[]= {1,6,5,0};
    num = process_record("????.######..#####.", 0, c4);
    assert(num == 4);
}