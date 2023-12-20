#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

#define BUFFER_SIZE     256

#define MAX_DEVICES     64

// 12345678901
// broadcaster
#define NAME_LEN        12

int ndevices;

enum module_type
{
    BROADCAST,
    FLIP_FLOP,
    CONJUNCTION,
};

#define MAX_OUTPUTS     8

struct module
{
    enum module_type type;
    char name[NAME_LEN];
    uint8_t id;
    uint8_t outputs[MAX_OUTPUTS];
    //uint64_t input_mask;
};

struct module modules[MAX_DEVICES];

static void parse_name(char *line);
static void parse_module(char *line);
static int dev_id_from_name(const char *name);

int main(void)
{
    char line[BUFFER_SIZE];
    FILE *input = fopen("input.txt", "r");

    memset(modules, 0, sizeof(modules));
    strcpy(modules[0].name, "broadcaster"); // reserve #0 for brodcaster
    ndevices = 1;

    for (;;)
    {
        if (fgets(line, BUFFER_SIZE, input) == NULL) break; 
        parse_name(line);
    }

    fseek(input, 0, SEEK_SET);

    for (;;)
    {
        if (fgets(line, BUFFER_SIZE, input) == NULL) break; 
        parse_module(line);
    }

    fclose(input);

    int total = 0;
    printf("Result: %d\n", total);

    return 0;
}


static void parse_name(char *line)
{
    char *savep;

    char *name = strtok_r(line, " ", &savep);

    // skip, because it is reserved as #0
    if (strcmp(name, "broadcaster") == 0)
        return;

    // not interested in type during this pass
    if (line[0] == '%' || line[0] == '&')
        ++line;

    assert(ndevices < MAX_DEVICES);
    assert(strlen(line) < NAME_LEN);
    strcpy(modules[ndevices].name, line);
    ++ndevices; 
}


static void parse_module(char *line)
{
    char *sp1;
    char *name = strtok_r(line, " ->", &sp1);

    enum module_type mtype;
    switch (name[0])
    {
    case '%':
        mtype = FLIP_FLOP;
        ++name;
        break;
    case '&':
        mtype = CONJUNCTION;
        ++name;
        break;
    default:
        mtype = BROADCAST;
        break;
    }

    int dev = dev_id_from_name(name);
    assert (dev >=0 && dev < ndevices);

    modules[dev].id = dev; //TODO: do I need it?
    modules[dev].type = mtype;

    int nout = 0;
    char *output = strtok_r(NULL, "->, \n", &sp1);
    while (output)
    {
        modules[dev].outputs[nout] = dev_id_from_name(output);
        ++nout;
        output = strtok_r(NULL, ", \n", &sp1);
    }
}

static int dev_id_from_name(const char *name)
{
    int i;
    for (i = 0; i < ndevices; ++i)
    {
        if (strcmp(name, modules[i].name) == 0)
            return i;
    }

    return -1;
}