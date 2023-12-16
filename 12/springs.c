#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>


#define BUFFER_SIZE     256
#define MAX_GROUPS       40
#define TEMPLATE_LEN    128

unsigned glob_groups[MAX_GROUPS];
long process_memo[TEMPLATE_LEN][MAX_GROUPS];

static long process_line(char *line);
static long process_record(const char template[], int start_idx, int group_idx);
static long process_record_memoized(const char template[], int start_idx, int group_idx);
static void reset_memo();
static void self_tests();

int main(void)
{
    self_tests();
    //return 0;

    char line[BUFFER_SIZE];
    FILE *input = fopen("input.txt", "r");

    long total = 0;

    for (;;)
    {
        if (fgets(line, BUFFER_SIZE, input) == NULL) break;
        total += process_line(line);
    }

    // p1 result: 7191
    // p2 result: 6512849198636
    printf("Result: %ld\n", total);

    fclose(input);
    return 0;
}


static long process_line(char *line)
{
    char *outersp, *innersp;
    char *template = strtok_r(line, " ", &outersp);

    //printf("%s ", template);

    char *groups_token = strtok_r(NULL, " \n", &outersp);

    //printf("%s ", groups_token);

    int num_groups = 0;

    char *token = strtok_r(groups_token, ",", &innersp);
    while (token) {
        glob_groups[num_groups++] = atoi(token);
        token = strtok_r(NULL, ",", &innersp);
    }
    glob_groups[num_groups] = 0;

    char unfolded[TEMPLATE_LEN];
    sprintf(unfolded, "%s?%s?%s?%s?%s", template, template, template, template, template);

    for (int i = 0; i < num_groups; ++i)
    {
        glob_groups[i + num_groups * 1] = glob_groups[i];
        glob_groups[i + num_groups * 2] = glob_groups[i];
        glob_groups[i + num_groups * 3] = glob_groups[i];
        glob_groups[i + num_groups * 4] = glob_groups[i];
    }
    glob_groups[num_groups * 5] = 0;

    // printf("%s ", unfolded);

    // for (int i = 0; groups[i]; ++i)
    //     printf("%d,", groups[i]);
    // printf("\n");

    reset_memo();
    long result = process_record_memoized(unfolded, 0, 0);

    //printf("%ld\n", result);

    // char *expected_p1_token = strtok_r(NULL, " \n", &outersp);
    // if (expected_p1_token)
    // {
    //     int expected = atoi(expected_p1_token);
    //     assert(expected == result);
    // }

    //printf("Unk: %d, groups: %d tlen: %lu\n", num_unknown, num_groups, strlen(template));
    return result;
}

static bool fill_group(const char *template, int grp);

static long process_record_memoized(const char template[], int start_idx, int group_idx)
{
    if (process_memo[start_idx][group_idx] == -1)
    {
        long result = process_record(template, start_idx, group_idx);
        process_memo[start_idx][group_idx] = result;
        //printf("R: (%d, %d) -> %ld\n", start_idx, group_idx, result);
        return result;
    }
    //printf("C: (%d, %d) -> %ld\n", start_idx, group_idx, process_memo[start_idx][group_idx]);
    return process_memo[start_idx][group_idx];
}

static long process_record(const char template[], int start_idx, int group_idx)
{
    unsigned grp = glob_groups[group_idx];

    if (grp == 0)
    {
        for (int i = start_idx; template[i]; ++i)
            if (template[i] == '#')
                return 0;
        return 1;
    }

    long sub_counts = 0;

    for (;;)
    {
        // scan forward until #, ? or \0
        for( ; template[start_idx] == '.'; ++start_idx);

        switch (template[start_idx])
        {
        case '#':
            /* arrived at beginning of known damage, must try to use all group */
            if (fill_group(template + start_idx, grp))
            {
                // good, go to next group
                int spacer = 0;
                if (template[start_idx + grp] == '?')
                {
                    spacer = 1;
                }
                return sub_counts + process_record_memoized(template, start_idx + grp + spacer, group_idx + 1);
            }
            else
                return sub_counts; // did not fit, stop trying
            break;
        case '?':
            /* arrived at the beginning of unknown, can try one by one */
            if (fill_group(template + start_idx, grp))
            {
                // good, try to go deeper
                int spacer = 0;
                if (template[start_idx + grp] == '?')
                {
                    spacer = 1;
                }

                sub_counts += process_record_memoized(template, start_idx + grp + spacer, group_idx + 1);
            }
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

static bool fill_group(const char *template, int grp)
{
    int p;
    for (p = 0; p < grp; ++p)
    {
        if (template[p] != '#' && template[p] != '?')
            /* did not fit */
            return false;
    }
    return template[p] != '#';
}


static void reset_memo()
{
    memset(process_memo, -1, sizeof(process_memo));
}


static void set_groups(unsigned *groups, ...)
{
    va_list args;
    va_start(args, groups);

    int count = 0;
    for(;;++count)
    {
        groups[count] = va_arg(args, unsigned);
        if (!groups[count]) break;
    }

    va_end(args);
}


static void self_tests()
{
    set_groups(glob_groups, 3, 0);

    // not fit
    reset_memo();
    long num = process_record("..#?", 0, 0);
    assert(num == 0);

    // fit exactly
    reset_memo();
    num = process_record("..##?", 0, 0);
    assert(num == 1);

    // fit with trailing good
    reset_memo();
    num = process_record("..##?.", 0, 0);
    assert(num == 1);

    // fit and unknown reminder
    reset_memo();
    num = process_record("..##??", 0, 0);
    assert(num == 1);

    // unknown start trailing #, 1 option
    reset_memo();
    num = process_record("..??##", 0, 0);
    assert(num == 1);

    // can't fit, result too long
    reset_memo();
    num = process_record("..#??#", 0, 0);
    assert(num == 0);

    // need separator
    reset_memo();
    set_groups(glob_groups, 1, 1, 0);
    num = process_record("???", 0, 0);
    assert(num == 1);

    // alternating
    // .#.###.#.######
    reset_memo();
    set_groups(glob_groups, 1, 3, 1, 6, 0);
    num = process_record("?#?#?#?#?#?#?#?", 0, 0);
    assert(num == 1);

    // ????.######..#####.
    reset_memo();
    set_groups(glob_groups, 1,6,5,0);
    num = process_record("????.######..#####.", 0, 0);
    assert(num == 4);

    // ?###???????? 3,2,1
    reset_memo();
    set_groups(glob_groups, 3,2,1,0);
    num = process_record("?###????????", 0, 0);
    assert(num == 10);

    //??#??????#???.? 4,3 9
    reset_memo();
    set_groups(glob_groups, 4,3,0);
    num = process_record("??#??????#???.?", 0, 0);
    assert(num == 9);

    // ????????.?##?????????????????.?##?????????????????.?##?????????????????.?##?????????????????.?##???????? 3,1,1,4,1,1,3,1,1,4,1,1,3,1,1,4,1,1,3,1,1,4,1,1,3,1,1,4,1,1,
    reset_memo();
    set_groups(glob_groups, 3,1,1,4,1,1,3,1,1,4,1,1,3,1,1,4,1,1,3,1,1,4,1,1,3,1,1,4,1,1,0);
    num = process_record("????????.?##?????????????????.?##?????????????????.?##?????????????????.?##?????????????????.?##????????", 0, 0);

    assert(num == 70220760064);
}