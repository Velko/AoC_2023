#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define BUFFER_SIZE     256
#define MAX_RANGES      200  /* there are ~220 lines total in input, should be enough */
#define MAX_SEEDS       100

struct map_range
{
    long offset;
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


struct map_range seed_to_soil_cached;
struct map_range soil_to_fertilizer_cached;
struct map_range fertilizer_to_water_cached;
struct map_range water_to_light_cached;
struct map_range light_to_temperature_cached;
struct map_range temperature_to_humidity_cached;
struct map_range humidity_to_location_cached;

static int parse_seeds(char *seeds);
static void read_map(FILE *input, struct map_range *range);
static long lookup(long source, struct map_range *range, struct map_range *cached);

int main(void)
{
    char line[BUFFER_SIZE];
    FILE *input = fopen("input.txt", "r");

    int num_seed_ranges = 0;

    for (;;)
    {
        if (fgets(line, BUFFER_SIZE, input) == NULL) break;

        if (strstr(line, "seeds: ") == line)
            num_seed_ranges = parse_seeds(line + 7);
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

    long total = 0;
    for (int sr = 0; sr < num_seed_ranges; ++sr)
    {
        total += seeds[sr].limit - seeds[sr].start;
    }

    seed_to_soil_cached.limit = 0;
    soil_to_fertilizer_cached.limit = 0;
    fertilizer_to_water_cached.limit = 0;
    water_to_light_cached.limit = 0;
    light_to_temperature_cached.limit = 0;
    temperature_to_humidity_cached.limit = 0;
    humidity_to_location_cached.limit = 0;

    printf("Total: %ld\n", total);

    long min_location = __LONG_MAX__;

    total /= 100;
    long progress = 0;
    long old_percent = 0;

    for (int sr = 0; sr < num_seed_ranges; ++sr)
    {
        for (long seed = seeds[sr].start; seed < seeds[sr].limit; ++seed)
        {
            ++progress;
            long percent = progress / total;
            if (percent != old_percent)
            {
                printf("%d%%\n", percent);
                old_percent = percent;
            }

            long soil = lookup(seed, seed_to_soil, &seed_to_soil_cached);
            long fertilizer = lookup(soil, soil_to_fertilizer, &soil_to_fertilizer_cached);
            long water = lookup(fertilizer, fertilizer_to_water, &fertilizer_to_water_cached);
            long light = lookup(water, water_to_light, &water_to_light_cached);
            long temperature = lookup(light, light_to_temperature, &light_to_temperature_cached);
            long humidity = lookup(temperature, temperature_to_humidity, &temperature_to_humidity_cached);
            long location = lookup(humidity, humidity_to_location, &humidity_to_location_cached);

            if (location < min_location) min_location = location;
        }
    }

    // result p1: 261668924
    // result p2: 24261545

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
        seed_token = strtok_r(NULL, " ", &savep);
        seeds[count].limit = seeds[count].start + atol(seed_token);

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
    long dest;

    for (;;++count) {
        int nread = fscanf(input, "%ld %ld %ld", &dest, &range[count].src, &length);
        if (nread != 3) break;
        range[count].limit = range[count].src + length;
        range[count].offset = dest - range[count].src;

        //printf("[%ld:%ld) -> %ld\n", range[count].src, range[count].limit, range[count].dest);
    }
    range[count].limit = 0;
}

static long lookup(long source, struct map_range *range, struct map_range *cached)
{
    // try cached first
    if (source >= cached->src && source < cached->limit)
        return source + cached->offset;

    // find new range
    for (int i = 0; range[i].limit != 0; ++i)
    {
        if (source >= range[i].src && source < range[i].limit)
        {
            memcpy(cached, range + i, sizeof(struct map_range));
            return source + range[i].offset;
        }
    }

    // not found, prepare new pseudo-cache - gap where *source* belongs:
    //     find largest limit which is smaller (or equal) than *source* - use as src
    //     find smallest src, that is larger than *source* - use as limit
    //     the offset remains 0
    cached->offset = 0;
    cached->src = 0;
    cached->limit = __LONG_MAX__;

    for (int i = 0; range[i].limit; ++i)
    {
        if (range[i].limit <= source && cached->src < range[i].limit)
            cached->src = range[i].limit;
        if (range[i].src > source && cached->limit > range[i].src)
            cached->limit = range[i].src;
    }

    return source;
}
