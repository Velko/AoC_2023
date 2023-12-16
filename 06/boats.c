#include <stdio.h>
#include <math.h>


struct race
{
    long time;
    long distance;
};

/* Algebraic solution:

   dist = v * t_travel
   v = charge_rate * t_charge
   t_total = t_travel + t_charge
   charge_rate = 1
   --------------------------------

   t_travel = t_total - t_charge
   dist = 1 * t_charge) * (t_total - t_charge)

   dist = t_charge * t_total - t_charge * t_charge
   -t_charge^2 + t_charge * t_total - dist = 0

   Quadratic equation, a = -1, b = t_total, c = -dist

   discriminant = t_total^2 - 4 * (-1) * (-dist)
   discriminant = t_total^2 - 4 * dist

   roots:
        (-t_total + sqrt(discriminant)) / -2
        (-t_total - sqrt(discriminant)) / -2

        r1 = (t_total - sqrt(discriminant)) / 2
        r2 = (t_total + sqrt(discriminant)) / 2

    result (distance between roots)
        r2 - r1
        (t_total + sqrt(discriminant)) / 2 - (t_total - sqrt(discriminant)) / 2
        (t_total + sqrt(discriminant) - t_total + sqrt(discriminant)) / 2
        (sqrt(discriminant) + sqrt(discriminant)) / 2
        sqrt(discriminant)

 */


struct race races[] =
{
//  { time: 1, distance: 2 },
};



static long try_race(struct race *r);

int main(void)
{
    long result = 1;
    for (int i = 0; i < 1; ++i)
    {
        result *= try_race(races + i);
    }

    printf("Result: %ld\n", result);
    return 0;
}

static long try_race(struct race *r)
{
    long discriminant = r->time * r->time - 4 * r->distance;
    return sqrt(discriminant);
}
