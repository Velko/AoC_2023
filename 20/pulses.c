#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <limits.h>

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
    RX,
};

#define MAX_OUTPUTS     8


struct pulse;
struct module
{
    enum module_type type;
    char name[NAME_LEN];
    int id;
    int outputs[MAX_OUTPUTS];
    uint64_t input_mask;
    void (*handle_pulse)(struct module *m, struct pulse *p);
    uint64_t state;
};

struct module modules[MAX_DEVICES];


enum level
{
    LOW = 0,
    HIGH = 1,
};

struct pulse
{
    enum level level;
    int source;
    int target;
};

#define BROADCASTER_ID      1
#define RX_ID               (BROADCASTER_ID + 1)

#define MAX_QUEUE   MAX_DEVICES

struct pulse queue[MAX_DEVICES];
int q_read_idx;
int q_write_idx;


int low_pulses_sent;
int high_pulses_sent;
long button_presses;

static void parse_name(char *line);
static void parse_module(char *line);
static int dev_id_from_name(const char *name);
static void wire_inputs();
static void print_dependencies();
static void send_pulse(enum level lvl, int source, int target);
static void push_button();
static void handle_rx(struct module *m, struct pulse *p);


int main(void)
{
    char line[BUFFER_SIZE];
    FILE *input = fopen("input.txt", "r");

    memset(modules, 0, sizeof(modules));
    strcpy(modules[BROADCASTER_ID].name, "broadcaster"); // reserve #0 for brodcaster
    strcpy(modules[RX_ID].name, "rx");
    ndevices = 3;

    modules[RX_ID].type = RX;
    modules[RX_ID].handle_pulse = handle_rx;

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

    wire_inputs();

    print_dependencies();

    q_read_idx = q_write_idx = 0;

    low_pulses_sent = high_pulses_sent = 0;

    for (button_presses = 1; button_presses < LONG_MAX; ++button_presses)
        push_button();

    printf("Low: %d, high: %d\n", low_pulses_sent, high_pulses_sent);

    long result = (long)low_pulses_sent * high_pulses_sent;

    // result p1: 814934624
    printf("Result: %ld\n", result);

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

static void handle_broadcast(struct module *m, struct pulse *p);
static void handle_flipflop(struct module *m, struct pulse *p);
static void handle_conjunction(struct module *m, struct pulse *p);


static void parse_module(char *line)
{
    char *sp1;
    char *name = strtok_r(line, " ->", &sp1);

    enum module_type mtype;
    void (*handler)(struct module *m, struct pulse *p);
    switch (name[0])
    {
    case '%':
        mtype = FLIP_FLOP;
        handler = handle_flipflop;
        ++name;
        break;
    case '&':
        mtype = CONJUNCTION;
        handler = handle_conjunction;
        ++name;
        break;
    default:
        mtype = BROADCAST;
        handler = handle_broadcast;
        break;
    }

    int dev = dev_id_from_name(name);
    assert (dev >=0 && dev < ndevices);

    modules[dev].id = dev;
    modules[dev].type = mtype;
    modules[dev].handle_pulse = handler;

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

static void wire_inputs()
{
    for (int m = 1; m < ndevices; ++m)
    {
        for (int o = 0; modules[m].outputs[o]; ++o)
        {
            int target = modules[m].outputs[o];
            modules[target].input_mask |= 1ULL << m;
        }
    }
}

static void print_dependencies()
{
    for (int m = 1; m < ndevices; ++m)
    {
        printf("%d %s (", modules[m].type, modules[m].name);
        for (int i = 1; i < ndevices; ++i)
        {
            if (modules[m].input_mask & (1ULL << i))
                printf("%s, ", modules[i].name);
        }

        printf(")\n");
    }
}

static void push_button()
{
    send_pulse(LOW, -1, BROADCASTER_ID); // no source to broadcaster

    while (q_read_idx < q_write_idx)
    {
        struct pulse *p = &queue[q_read_idx % MAX_QUEUE];
        if (p->target != -1)
        {
            assert(p->target < ndevices);
            
            struct module *m = &modules[p->target];

            m->handle_pulse(m, p);
        }
        ++q_read_idx;
    }
}

static void send_pulse(enum level lvl, int source, int target)
{
    assert(q_write_idx - q_read_idx < MAX_QUEUE);

    // char *src = source != -1 ? modules[source].name : "(none)";
    // char *tgt = target != -1 ? modules[target].name : "(none)";
    // char *l = lvl ? "high" : "low";

    // printf("%s -%s-> %s\n", src, l, tgt);

    if (lvl == LOW)
        ++low_pulses_sent;
    else
        ++high_pulses_sent;

    int idx = q_write_idx % MAX_QUEUE;
    queue[idx].level = lvl;
    queue[idx].source = source;
    queue[idx].target = target;
    ++q_write_idx;
}

static void handle_broadcast(struct module *m, struct pulse *p)
{
    for (int o = 0; m->outputs[o]; ++o)
        send_pulse(p->level, m->id, m->outputs[o]);
}

static void handle_flipflop(struct module *m, struct pulse *p)
{
    assert(m->type == FLIP_FLOP);
    if (p->level == LOW)
    {
        printf("%s  @ %ld\n", m->name, button_presses);
        m->state ^= 1;
        p->level = m->state ? HIGH : LOW;
        handle_broadcast(m, p);
    }
}

static void handle_conjunction(struct module *m, struct pulse *p)
{
    assert(m->type == CONJUNCTION);
    assert(p->source != -1 && p->source < MAX_DEVICES);

    if (p->level == LOW)
        m->state &= ~(1ULL << p->source);
    else
        m->state |= 1ULL << p->source;

    p->level = m->state == m->input_mask ? LOW : HIGH;

    handle_broadcast(m, p);
}

static void handle_rx(struct module *m, struct pulse *p)
{
    assert(m->type == RX);
    if (p->level == LOW)
    {
        printf("RX reached: %ld\n", button_presses);
        exit(0);
    }
}