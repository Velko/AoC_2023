#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#define BUFFER_SIZE     256

#define MAX_INSTRUCTIONS    700

struct instr
{
    char dir;
    long steps;
};

struct instr instructions[MAX_INSTRUCTIONS];
int ninstructions;


struct vertice
{
    long row;
    long col;
};

struct vertice vertices[MAX_INSTRUCTIONS];
int nvertices;


static void parse_line(char *line);
static long calculate_polygon_area();
static long get_points_inside(long area, long nboundary);
static long walk_around();

int main(void)
{
    char line[BUFFER_SIZE];
    FILE *input = fopen("input.txt", "r");

    for (ninstructions = 0;; ++ninstructions)
    {
        if (fgets(line, BUFFER_SIZE, input) == NULL) break; 
        parse_line(line);
    }
    fclose(input);


    long perimeter = walk_around();
    long area = calculate_polygon_area();
    long points = get_points_inside(area, perimeter);

    long result = points + perimeter;

    // result p1: 40131
    // result p2: 104454050898331
    printf("Result: %ld\n", result);
    return 0;
}


static void parse_line(char *line)
{
    char *savep;

    strtok_r(line, "#", &savep);
    char *color = strtok_r(NULL, ")", &savep);

    instructions[ninstructions].dir = color[5];

    color[5] = 0;
    instructions[ninstructions].steps = strtol(color, NULL, 16);

    //sscanf(line, "%c %d", &instructions[ninstructions].dir, &instructions[ninstructions].steps);
    //printf("%05lx %c\n", instructions[ninstructions].steps, instructions[ninstructions].dir);
}

static long walk_around()
{
    long perimeter = 0;
    long row = 0;
    long col = 0;

    vertices[0].row = row;
    vertices[0].col = col;
    nvertices = 1;

    // 0 means R, 1 means D, 2 means L, and 3 means U.
    for (int i = 0; i < ninstructions; ++i)
    {
        switch (instructions[i].dir)
        {
        case 'D':
        case '1':
            row += instructions[i].steps;
            break;
        case 'U':
        case '3':
            row -= instructions[i].steps;
            break;
        case 'R':
        case '0':
            col += instructions[i].steps;
            break;
        case 'L':
        case '2':
            col -= instructions[i].steps;
            break;
        default:
            printf("WTF\n");
            exit(1);
            break;
        }
        vertices[nvertices].row = row;
        vertices[nvertices].col = col;
        ++nvertices;

        perimeter += instructions[i].steps;

        //printf("(%ld, %ld)\n", row, col);
    }

    return perimeter;
}

static long calculate_polygon_area()
{
    // https://en.m.wikipedia.org/wiki/Shoelace_formula#Triangle_formula
    long area = 0;

    for (int i = 1; i < nvertices; ++i)
    {
        area += vertices[i - 1].col * vertices[i].row - vertices[i].col * vertices[i - 1].row;
    }

    return area / 2;
}

static long get_points_inside(long area, long nboundary)
{
    // https://en.wikipedia.org/wiki/Pick%27s_theorem
    /*
        The formula is:
        A = i + b/2 -1
        A - area
        i - # of interior points
        b - # of boundary points (whole integers that belong to perimeter)

        We need to obtain the i - # of interior points.

        i = A - b/2 + 1

        as the polygon consists only from integer number of steps and right angle
        turns, all the points of perimeter belongs to it
     */

    return area - nboundary / 2 + 1;
}