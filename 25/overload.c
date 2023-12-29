#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

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

#define MAX_LINKS       3500
struct link_item
{
    int src;
    int dest;
};

struct link_item link_list[MAX_LINKS];
int nlinks;

static void add_to_index(char *line);
static void mark_links(char *line);
static int find_in_index(const char *name);
static void print_matrix();
static int count_visitable();
static void build_linklist();
static void print_dot();

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

    //print_dot();
    //return 0;
    build_linklist();
    //print_links();

    printf("# links: %d\n", nlinks);

    //int total = count_visitable();

    long progress = 0;
    // for (int r = 0; r < ncomponents; ++r)
    // {
    //     for (int c = r + 1; c < ncomponents; ++c)
    //     {
    //         total+=links[r][c];
    //     }
    // }

    for (int a = 0; a < nlinks; ++a)
    {
        struct link_item *la = &link_list[a];

        //cut link
        links[la->src][la->dest] = false;
        links[la->dest][la->src] = false;

        int visitable = count_visitable();
        if (visitable != ncomponents)
        {
            printf("cut %s/%s\n", components[la->src].name, components[la->dest].name);
            printf("Clusters: %d %d\n", visitable, ncomponents - visitable);
            printf("Result: %d\n", visitable * (ncomponents - visitable));
            exit(0);
        }

        // put it back
        links[la->src][la->dest] = true;
        links[la->dest][la->src] = true;
    }
    //printf("Result: %ld\n", nlinks);


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


int visits[MAX_COMPONENTS];
static int count_visitable()
{
    memset(visits, 0, sizeof(visits));
    visits[0] = 1;

    int old_visited = 0;
    int nvisited = 1;
    for (int visit_id = 1; ; ++visit_id)
    {
        for (int src = 0; src < ncomponents; ++src)
        {
            if (visits[src] == visit_id)
            {
                for (int dest = 0; dest < ncomponents; ++dest)
                {
                    if (links[src][dest] && visits[dest] == 0)
                    {
                        visits[dest] = visit_id + 1;
                        ++nvisited;
                    }
                }
            }
        }
        if (old_visited == nvisited) return nvisited;
        old_visited = nvisited;
    }
}

static void build_linklist()
{
    nlinks = 0;
    for (int r = 0; r < ncomponents; ++r)
    {
        for (int c = r + 1; c < ncomponents; ++c)
        {
            if (links[r][c])
            {
                assert(nlinks < MAX_LINKS);
                link_list[nlinks].src = r;
                link_list[nlinks].dest = c;
                ++nlinks;
            }
        }
    }
}

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