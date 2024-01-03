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
static int walk_all_nodes(int start, int *last_visit);
static void remove_path(int start, int target);

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

    int nvisited = 0;

    /* This assumes that path found by BFS to the furthest node from *start* "crosses the bridge" at least once.
       Removing this path removes a single connection between the 2 subgraphs (along with bunch of other links, we do
       not care much about). Repeating this 2 more times should remove other 2 bridges. Other nodes that were removed
       reduces connectivity a bit, but nodes should still remain reachable.

       If, for some reason, this is not true, another node should be picked (currently this is not addressed in code).

       Then we're doing one last walk to discover how many nodes now are reachable from the *start*.
     */
    int start = 0;
    for (int i = 0; i < NPATHS; ++i)
    {
        int last;
        nvisited = walk_all_nodes(start, &last);
        remove_path(start, last);
    }

    printf("%d * %d\n", nvisited, ncomponents - nvisited);

    int result = nvisited * (ncomponents - nvisited);

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


static void remove_path(int start, int target)
{
    for (int s = target; s != start; s = visits[s].from)
    {
        links[s][visits[s].from] = false;
        links[visits[s].from][s] = false;

        #ifdef DEBUG_PRINT
        if (s == target)
            printf("[%s]", components[s].name);
        else
            printf(" <- %s", components[s].name);
        #endif
    }

    #ifdef DEBUG_PRINT
    printf(" <- [%s]\n", components[start].name);
    #endif
}

static int walk_all_nodes(int start, int *last_visit)
{
    memset(visits, 0, sizeof(visits));
    visits[start].visit_id = 1;
    int nvisited = 1;

    int old_nvisited = 0;
    for (int visit_id = 1; old_nvisited != nvisited; ++visit_id)
    {
        old_nvisited = nvisited;
        for (int src = 0; src < ncomponents; ++src)
        {
            if (visits[src].visit_id == visit_id)
            {
                for (int dest = 0; dest < ncomponents; ++dest)
                {
                    if (links[src][dest])
                    {
                        if (visits[dest].visit_id == 0)
                        {
                            visits[dest].visit_id = visit_id + 1;
                            visits[dest].from = src;
                            *last_visit = dest;
                            ++nvisited;
                        }
                    }
                }
            }
        }
    }

    return nvisited;
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