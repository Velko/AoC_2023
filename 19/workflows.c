#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#define BUFFER_SIZE     256


#define MAX_WORKFLOWS   520
#define MAX_STEPS         8
#define WF_NAME_LEN       4


struct workflow
{
    char name[WF_NAME_LEN];
    struct {
        char param;
        char op;
        int value;
        char target[WF_NAME_LEN];
    } steps[MAX_STEPS];
};

struct workflow workflows[MAX_WORKFLOWS];
int nworkflows = 0;


static void parse_workflow(char *line);

int main(void)
{
    char line[BUFFER_SIZE];
    FILE *input = fopen("input.txt", "r");

    int total = 0;

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

    //printf("Result: %d\n", total);

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
            printf("%s\n", condition);
        }

        ++nsteps;
        step = strtok_r(NULL, ",", &sp2);
    }

    ++nworkflows;
}