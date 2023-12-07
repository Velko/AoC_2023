#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define BUFFER_SIZE     256
#define MAX_RANGES      200  /* there are ~220 lines total in input, should be enough */
#define MAX_SEEDS       100

struct map_range
{
    long dest;
    long src;
    long limit;
};

struct seed_range
{
    long start;
    long limit;
};

struct seed_range seeds[MAX_SEEDS];
struct map_range seed_to_soil[MAX_RANGES];
struct map_range soil_to_fertilizer[MAX_RANGES];
struct map_range fertilizer_to_water[MAX_RANGES];
struct map_range water_to_light[MAX_RANGES];
struct map_range light_to_temperature[MAX_RANGES];
struct map_range temperature_to_humidity[MAX_RANGES];
struct map_range humidity_to_location[MAX_RANGES];

static int parse_seeds(char *seeds);
static void read_map(FILE *input, struct map_range *range);
static long lookup(long source, struct map_range *range);

int main(void)
{
    char line[BUFFER_SIZE];
    FILE *input = fopen("input.txt", "r");

    int num_seeds = 0;

    for (;;)
    {
        if (fgets(line, BUFFER_SIZE, input) == NULL) break;

        if (strstr(line, "seeds: ") == line)
            num_seeds = parse_seeds(line + 7);
        else if (strstr(line, "seed-to-soil map:") == line)
            read_map(input, seed_to_soil);
        else if (strstr(line, "soil-to-fertilizer map:") == line)
            read_map(input, soil_to_fertilizer);
        else if (strstr(line, "fertilizer-to-water map:") == line)
            read_map(input, fertilizer_to_water);
        else if (strstr(line, "water-to-light map:") == line)
            read_map(input, water_to_light);
        else if (strstr(line, "light-to-temperature map:") == line)
            read_map(input, light_to_temperature);
        else if (strstr(line, "temperature-to-humidity map:") == line)
            read_map(input, temperature_to_humidity);
        else if (strstr(line, "humidity-to-location map:") == line)
            read_map(input, humidity_to_location);
    }

    fclose(input);

    long min_location = __LONG_MAX__;

    for (int s = 0; s < num_seeds; ++s)
    {
        long soil = lookup(seeds[s].start, seed_to_soil);
        long fertilizer = lookup(soil, soil_to_fertilizer);
        long water = lookup(fertilizer, fertilizer_to_water);
        long light = lookup(water, water_to_light);
        long temperature = lookup(light, light_to_temperature);
        long humidity = lookup(temperature, temperature_to_humidity);
        long location = lookup(humidity, humidity_to_location);

        if (location < min_location) min_location = location;
    }

    printf("Result: %ld\n", min_location);

    return 0;
}

static int parse_seeds(char *seed_str)
{
    char *savep;
    char *seed_token = strtok_r(seed_str, " ", &savep);

    int count = 0;
    while (seed_token)
    {
        seeds[count].start = atol(seed_token);
        seeds[count].limit = 1;
        //seed_token = strtok_r(NULL, " ", &savep);
        //seeds[count].limit = seeds[count].start + atol(seed_token);

        seed_token = strtok_r(NULL, " ", &savep);
        ++count;
    }

    printf("S: %d\n", count);

    return count;
}

static void read_map(FILE *input, struct map_range *range)
{
    int count = 0;
    long length;

    for (;;++count) {
        int nread = fscanf(input, "%ld %ld %ld", &range[count].dest, &range[count].src, &length);
        if (nread != 3) break;
        range[count].limit = range[count].src + length;

        //printf("[%ld:%ld) -> %ld\n", range[count].src, range[count].limit, range[count].dest);
    }
    range[count].limit = 0;
}

static long lookup(long source, struct map_range *range)
{
    for (int i = 0; range[i].limit != 0; ++i)
    {
        if (source >= range[i].src && source < range[i].limit)
            return range[i].dest + (source - range[i].src);
    }

    return source;
}
