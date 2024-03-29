#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#define BUFFER_SIZE     256


#define MAX_WORKFLOWS   520
#define MAX_STEPS         8
#define WF_NAME_LEN       4
#define NPARAMS           4


struct wf_step
{
    char param;
    char op;
    int value;
    char target[WF_NAME_LEN];
};

struct workflow
{
    char name[WF_NAME_LEN];
    struct wf_step steps[MAX_STEPS];
};

struct workflow workflows[MAX_WORKFLOWS];
int nworkflows = 0;


const char *val_names = "xmas";
struct part
{
    int values[NPARAMS];
};

struct range
{
    int start;
    int limit;
};


struct ranges
{
    struct range r[NPARAMS];
};


static void parse_workflow(char *line);
static int process_part(char *line);

static long check_split_ranges();

int main(void)
{
    char line[BUFFER_SIZE];
    FILE *input = fopen("input.txt", "r");

    for (;;)
    {
        if (fgets(line, BUFFER_SIZE, input) == NULL)
        {
            printf("WTF\n");
            exit(1);
        }
        if (line[0] == '\n')
            break;
        parse_workflow(line);
    }

    int total = 0;
    for (;;)
    {
        if (fgets(line, BUFFER_SIZE, input) == NULL)
            break;
        total += process_part(line);
    }

    // result p1: 331208
    printf("Result p1: %d\n", total);

    // result p2: 121464316215623
    long result2 = check_split_ranges();
    printf("Result p2: %ld\n", result2);

    fclose(input);
    return 0;
}


static void parse_workflow(char *line)
{
    // qn{a<2099:cb,x<1109:llc,a>2465:zc,A}
    // tz{s>2124:R,x<1437:A,s>2018:A,R}
    char *sp1;
    char *name = strtok_r(line, "{", &sp1);

    
    assert(strlen(name) < WF_NAME_LEN);
    strcpy(workflows[nworkflows].name, name);

    char *contents = strtok_r(NULL, "}", &sp1);

    int nsteps = 0;
    char *sp2;
    char *step = strtok_r(contents, ",", &sp2);
    while (step)
    {
        char *sp3;
        char *condition = strtok_r(step, ":", &sp3);
        char *target =  strtok_r(NULL, ":", &sp3);

        if (target)
        {
            workflows[nworkflows].steps[nsteps].param = condition[0];
            workflows[nworkflows].steps[nsteps].op = condition[1];
            workflows[nworkflows].steps[nsteps].value = atoi(condition + 2);

            assert(strlen(target) < WF_NAME_LEN);
            strcpy(workflows[nworkflows].steps[nsteps].target, target);
        }
        else
        {
            // did not split, it's just a target
            assert(strlen(condition) < WF_NAME_LEN);
            strcpy(workflows[nworkflows].steps[nsteps].target, condition);
            workflows[nworkflows].steps[nsteps].op = '!';
        }

        ++nsteps;
        step = strtok_r(NULL, ",", &sp2);
    }

    ++nworkflows;
}


static int param_from_char(char p);
static bool run_wf(struct part *part);
static int process_part(char *line)
{
    struct part part = { .values = {0, 0, 0, 0 } };

    //{x=903,m=143,a=1348,s=25}
    ++line;
    line[strlen(line)-2] = 0; //strip } and \n


    char *sp;
    char *spec = strtok_r(line, ",", &sp);
    while (spec)
    {
        int pidx = param_from_char(spec[0]);

        assert(spec[1] == '=');

        part.values[pidx] = atoi(spec + 2);

        spec = strtok_r(NULL, ",", &sp);
    }

    bool accepted = run_wf(&part);

    if (accepted)
    {
        //printf("%d\n", part->values[0] + part->values[1] + part->values[2] + part->values[3]);
        return part.values[0] + part.values[1] + part.values[2] + part.values[3];
    }

    return 0;
}

static int param_from_char(char p)
{
    char *pp = strchr(val_names, p);
    assert(pp != NULL);
    return pp - val_names;
}


static struct workflow *lookup_workflow(const char *wfn)
{
    struct workflow *flow = NULL;
    for (int w = 0; w < nworkflows; ++w)
    {
        if (strcmp(workflows[w].name, wfn) == 0)
        {
            flow = workflows + w;
            break;
        }
    }

    assert (flow != NULL);
    return flow;
}

static const char *process_steps(struct wf_step *steps, int *part_values);
static bool run_wf(struct part *part)
{
    const char *wfn = "in";

    while (wfn[0] != 'A' && wfn[0] != 'R')
    {
        //printf("%s -> ", wfn);
        struct workflow *flow = lookup_workflow(wfn);

        wfn = process_steps(flow->steps, part->values);
    }

    //printf("%s\n", wfn);

    return wfn[0] == 'A';
}

static const char *check_step_condition(struct wf_step *step, int *part_values, int pidx);
static const char *process_steps(struct wf_step *steps, int *part_values)
{
    for (int s = 0; ; ++s)
    {
        int pidx = param_from_char(steps[s].param);
        const char *new_wf = check_step_condition(steps + s, part_values, pidx);
        if (new_wf)
            return new_wf;
    }
}

static const char *check_step_condition(struct wf_step *step, int *part_values, int pidx)
{
    switch (step->op)
    {
    case '<':
        if (part_values[pidx] < step->value)
            return step->target;
        break;
    case '>':
        if (part_values[pidx] > step->value)
            return step->target;
        break;
    case '!':
        return step->target;
    default:
        printf("WTF\n");
        exit(1);
        break;
    }
    return NULL;
}


static long check_split_workflow(const char *wfname, struct ranges *primary);

static long check_split_ranges()
{
    struct ranges primary;
    for (int r = 0; r < NPARAMS; ++r)
    {
        primary.r[r].start = 1;
        primary.r[r].limit = 4001; // limit is exclusive
    }

    return check_split_workflow("in", &primary);
}

static long check_split_workflow(const char *wfname, struct ranges *primary)
{
    if (wfname[0] == 'R')
        return 0;
    
    if (wfname[0] == 'A')
    {
        long num_ranges = 1;
        //printf("Accepting: \n");
        for (int p = 0; p < NPARAMS; ++p)
        {
            //printf("%c  [%d, %d)\n", val_names[p], primary->r[p].start, primary->r[p].limit);
            num_ranges *= primary->r[p].limit - primary->r[p].start;
        }

        //printf("Total: %ld ranges\n\n", num_ranges);
        return num_ranges;
    }

    struct workflow *flow = lookup_workflow(wfname);

    long total_ranges = 0;

    for (int s = 0; ; ++s)
    {
        if (flow->steps[s].op == '!')
        {
            return total_ranges + check_split_workflow(flow->steps[s].target, primary);
        }

        struct ranges branch;
        memcpy(&branch, primary, sizeof(struct ranges));

        int pidx = param_from_char(flow->steps[s].param);

        switch (flow->steps[s].op)
        {
        case '<':
            branch.r[pidx].limit = flow->steps[s].value;
            total_ranges += check_split_workflow(flow->steps[s].target, &branch);
            primary->r[pidx].start = flow->steps[s].value;
            break;
        case '>':
            branch.r[pidx].start = flow->steps[s].value + 1;
            total_ranges += check_split_workflow(flow->steps[s].target, &branch);
            primary->r[pidx].limit = flow->steps[s].value + 1;
            break;
        default:
            printf("WTF\n");
            exit(1);
            break;
        }
    }
}