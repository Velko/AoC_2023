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
    strcpy(reconstructed, template);

    unsigned grp = *groups;

    if (grp == 0)
    {
        for (int i = start_idx; template[i]; ++i)
            if (template[i] == '#')
                return 0;
        return 1;
    }

    int sub_counts = 0;

    for (;;)
    {
        // scan forward until #, ? or \0
        for( ; template[start_idx] == '.'; ++start_idx);

        switch (template[start_idx])
        {
        case '#':
            /* arrived at beginning of known damage, must try to use all group */
            if (fill_group(reconstructed + start_idx, template + start_idx, grp))
            {
                // good, go to next group
                int spacer = 0;
                if (template[start_idx + grp] == '?')
                {
                    reconstructed[start_idx + grp] = '.';
                    spacer = 1;
                }
                return sub_counts + process_record(reconstructed, start_idx + grp + spacer, groups + 1);
            }
            else
                return sub_counts; // did not fit, stop trying
            break;
        case '?':
            /* arrived at the beginning of unknown, can try one by one */
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
            ++start_idx;
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

static void self_tests()
{
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

    // ????.######..#####.
    unsigned c4[]= {1,6,5,0};
    num = process_record("????.######..#####.", 0, c4);
    assert(num == 4);

    // ?###???????? 3,2,1
    unsigned c5[]= {3,2,1,0};
    num = process_record("?###????????", 0, c5);
    assert(num == 10);

    //??#??????#???.? 4,3 9
    unsigned c6[]= {4,3,0};
    num = process_record("??#??????#???.?", 0, c6);
    assert(num == 9);

}