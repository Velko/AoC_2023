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

char device_index[MAX_DEVICES][NAME_LEN];
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
    uint8_t id;
    uint8_t outputs[MAX_OUTPUTS];
    //uint64_t input_mask;
};

int nmodules;


struct module modules[MAX_DEVICES];

static void parse_name(char *line);
static void parse_module(char *line);
static int dev_id_from_name(const char *name);

int main(void)
{
    char line[BUFFER_SIZE];
    FILE *input = fopen("input.txt", "r");

    memset(device_index, 0, sizeof(device_index));
    strcpy(device_index[0], "broadcaster"); // reserve #0 for brodcaster
    ndevices = 1;

    for (;;)
    {
        if (fgets(line, BUFFER_SIZE, input) == NULL) break; 
        parse_name(line);
    }

    nmodules = 0;
    memset(modules, 0, sizeof(modules));
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
    strcpy(device_index[ndevices], line);
    ++ndevices; 
}


static void parse_module(char *line)
{
    char *sp1;
    char *name = strtok_r(line, " ->", &sp1);

    assert(nmodules < MAX_DEVICES);

    switch (name[0])
    {
    case '%':
        modules[nmodules].type = FLIP_FLOP;
        ++name;
        break;
    case '&':
        modules[nmodules].type = CONJUNCTION;
        ++name;
        break;
    default:
        modules[nmodules].type = BROADCAST;
        break;
    }

    modules[nmodules].id = dev_id_from_name(name);

    int nout = 0;
    char *output = strtok_r(NULL, "->, \n", &sp1);
    while (output)
    {
        modules[nmodules].outputs[nout] = dev_id_from_name(output);
        ++nout;
        output = strtok_r(NULL, ", \n", &sp1);
    }
}

static int dev_id_from_name(const char *name)
{
    int i;
    for (i = 0; i < ndevices; ++i)
    {
        if (strcmp(name, device_index[i]) == 0)
            return i;
    }

    printf("%s\n", name);
//    assert(i < ndevices);
    return -1;
}