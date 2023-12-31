#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

//#define DEBUG_PRINT


#define BUFFER_SIZE     256

#define MAX_COMPONENTS  1600
#define MAX_NAME           4

struct component
{
    char name[MAX_NAME];
    int group;
};


struct component components[MAX_COMPONENTS];
int ncomponents;


bool links[MAX_COMPONENTS][MAX_COMPONENTS];

struct visit_item
{
    int visit_id;
    int from;
};

struct visit_item visits[MAX_COMPONENTS];

#define NPATHS  4

static void add_to_index(char *line);
static void mark_links(char *line);
static int find_in_index(const char *name);
static bool find_way_to_node(int start, int target, bool allow_direct);
static bool can_be_reached_n_ways(int start, int target);

#ifdef DEBUG_PRINT
static void print_matrix();
static void print_dot();
#endif

int main(void)
{
    char line[BUFFER_SIZE];
    FILE *input = fopen("input.txt", "r");

    ncomponents = 0;
    for (;;)
    {
        if (fgets(line, BUFFER_SIZE, input) == NULL) break; 
        add_to_index(line);
    }

    fseek(input, 0, SEEK_SET);
    memset(links, 0, sizeof(links));
    for (;;)
    {
        if (fgets(line, BUFFER_SIZE, input) == NULL) break; 
        mark_links(line);
    }
    fclose(input);

    #ifdef DEBUG_PRINT
    print_dot();
    print_matrix();
    #endif

    int src = 0; // pick start
    components[src].group = 1;
    for (int dest = src + 1; dest < ncomponents; ++dest)
    {
        bool reachable = can_be_reached_n_ways(src, dest);

        if (reachable)
        {
            #ifdef DEBUG_PRINT
            printf("Reachable\n");
            #endif
            components[dest].group = 1;
        }
        else
        {
            #ifdef DEBUG_PRINT
            printf("Unreachable\n");
            #endif
            components[dest].group = 2;
        }
    }

    int count1 = 0;
    int count2 = 0;

    for (int n = 0; n < ncomponents; ++n)
    {
        switch (components[n].group)
        {
        case 1:
            ++count1;
            break;
        case 2:
            ++count2;
            break;
        default:
            printf("WTF?\n");
            exit(1);
        }
    }

    printf("%d * %d\n", count1, count2);

    int result = count1 * count2;

    // Result: 582590
    printf("Result: %d\n", result);
    return 0;
}


static void add_to_index(char *line)
{
    char *sp;
    char *token = strtok_r(line, ": \n", &sp);
    while (token)
    {
        int idx = find_in_index(token);
        if (idx == -1)
        {
            assert(strlen(token) < MAX_NAME);
            strcpy(components[ncomponents].name, token);
            components[ncomponents].group = 0;
            ++ncomponents;
        }

        token = strtok_r(NULL, " \n", &sp);
    }
}

static void mark_links(char *line)
{
    char *sp;
    char *token = strtok_r(line, ": \n", &sp);
    int src = find_in_index(token);
    assert (src >=0 && src < MAX_COMPONENTS);
    token = strtok_r(NULL, " \n", &sp);
    while (token)
    {
        int dest = find_in_index(token);
        assert (dest >=0 && dest < MAX_COMPONENTS);

        links[src][dest] = true;
        links[dest][src] = true;

        token = strtok_r(NULL, " \n", &sp);
    }
}

static int find_in_index(const char *name)
{
    for (int i = 0; i < ncomponents; ++i)
    {
        if (strcmp(name, components[i].name) == 0)
            return i;
    }

    return -1;
}


bool blocked[MAX_COMPONENTS];

static bool can_be_reached_n_ways(int start, int target)
{
    memset(blocked, 0, sizeof(blocked));

    #ifdef DEBUG_PRINT
    printf("%s -> %s\n", components[start].name, components[target].name);
    #endif

    for (int n = 0; n < NPATHS; ++n)
    {
        bool arrived = find_way_to_node(start, target, n == 0);

        if (!arrived)
            return false;

        #ifdef DEBUG_PRINT
        printf("[%s]", components[target].name);
        #endif

        for (int s = visits[target].from; s != start; s = visits[s].from)
        {
            blocked[s] = true;
            #ifdef DEBUG_PRINT
            printf(" <- %s", components[s].name);
            #endif
        }

        #ifdef DEBUG_PRINT
        printf(" <- [%s]\n", components[start].name);
        #endif
    }

    return true;
}

static bool find_way_to_node(int start, int target, bool allow_direct)
{
    memset(visits, 0, sizeof(visits));
    visits[start].visit_id = 1;

    bool has_visitable = true;
    for (int visit_id = 1; has_visitable; ++visit_id)
    {
        has_visitable = false;
        for (int src = 0; src < ncomponents; ++src)
        {
            if (visits[src].visit_id == visit_id)
            {
                for (int dest = 0; dest < ncomponents; ++dest)
                {
                    if (links[src][dest])
                    {
                        if (target == dest && visit_id == 1 && !allow_direct)
                            continue;
                        if (blocked[dest])
                            continue;

                        
                        if (visits[dest].visit_id == 0)
                        {
                            visits[dest].visit_id = visit_id + 1;
                            visits[dest].from = src;
                            has_visitable = true;

                            if (target == dest)
                                return true;
                        }
                    }
                }
            }
        }
    }

    return false;
}


#ifdef DEBUG_PRINT

static void print_matrix()
{
    for (int x = 0; x < 3; ++x)
    {
        printf("    ");
        for (int c = 0; c < ncomponents; ++c)
            printf("%c ", components[c].name[x]);
        printf("\n");
    }
    for (int r = 0; r < ncomponents; ++r)
    {
        printf("%s ", components[r].name);
        for (int c = 0; c < ncomponents; ++c)
        {
            if (c <= r)
                printf("  ");
            else
                printf("%d ", links[r][c]);
        }
        printf("\n");
    }
}

static void print_dot()
{
    printf("graph {\n");
    for (int r = 0; r < ncomponents; ++r)
    {
        for (int c = r + 1; c < ncomponents; ++c)
        {
            if (links[r][c])
            {
                printf("    %s -- %s\n", components[r].name, components[c].name);
            }
        }
    }
    printf("}\n");
}

#endif