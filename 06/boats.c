#include <stdio.h>


struct race
{
    long time;
    long distance;
};


struct race races[] =
{
//  { time: 1, distance: 2 },
};



static int try_race(struct race *r);

int main(void)
{
    int result = 1;
    for (int i = 0; i < 1; ++i)
    {
        result *= try_race(races + i);
    }

    printf("Result: %d\n", result);
    return 0;
}

static int try_race(struct race *r)
{
    int win_count = 0;

    for (long t_load = 0; t_load < r->time; ++t_load)
    {
        long v = t_load;
        long t_travel = r->time - t_load;

        if (v * t_travel > r->distance)
            ++win_count;
    }


    return win_count;
}
