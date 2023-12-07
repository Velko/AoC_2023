#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Compile with -O3 for max performance. e.g
    clang -O3 seeds.c -o seeds

   Measure execution speed (on GNU/Linux):
    time ./seeds
 */

#define LINE_SIZE     256
#define MAX_MAPS       50  /* input contains about ~220 lines total. 50 per table should be is more than enough  */
#define MAX_SEEDS      50

// progress indicator is interesting but slows things down
//#define DISPLAY_PROGRESS

struct map_range
{
    long offset; //     precalculated as dest - src, optimization for source -> destination transformation
    long src;
    long limit;  //     precalculated as src + length, optimization for lookups
};

struct seed_range
{
    long start;
    long limit;  //     also precalculated as start + count, mostly for convenience, no real performance benefits
};

struct seed_range seeds[MAX_SEEDS];

/* maps are "null-terminated": an item with *limit* == 0 marks the end */
struct map_range seed_to_soil[MAX_MAPS];
struct map_range soil_to_fertilizer[MAX_MAPS];
struct map_range fertilizer_to_water[MAX_MAPS];
struct map_range water_to_light[MAX_MAPS];
struct map_range light_to_temperature[MAX_MAPS];
struct map_range temperature_to_humidity[MAX_MAPS];
struct map_range humidity_to_location[MAX_MAPS];

/* "secret sauce" for fast sequential lookups - cache recently used range */
struct map_range seed_to_soil_cache;
struct map_range soil_to_fertilizer_cache;
struct map_range fertilizer_to_water_cache;
struct map_range water_to_light_cache;
struct map_range light_to_temperature_cache;
struct map_range temperature_to_humidity_cache;
struct map_range humidity_to_location_cache;


static int parse_seeds(char *seeds);
static void read_map(FILE *input, struct map_range *mappings);
static long lookup(long source, struct map_range *mappings, struct map_range *cache);

int main(void)
{
    char line[LINE_SIZE];
    FILE *input = fopen("input.txt", "r");

    /* parse input file */
    int num_seed_ranges = 0;
    for (;;)
    {
        if (fgets(line, LINE_SIZE, input) == NULL) break;

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

    /* set caches to guaranteed miss during first lookup */
    seed_to_soil_cache.limit = 0;
    soil_to_fertilizer_cache.limit = 0;
    fertilizer_to_water_cache.limit = 0;
    water_to_light_cache.limit = 0;
    light_to_temperature_cache.limit = 0;
    temperature_to_humidity_cache.limit = 0;
    humidity_to_location_cache.limit = 0;

    #ifdef DISPLAY_PROGRESS
    long total = 0;
    for (int sr = 0; sr < num_seed_ranges; ++sr)
    {
        total += seeds[sr].limit - seeds[sr].start;
    }
    total /= 100;
    long progress = 0;
    long old_percent = 0;
    #endif

    long min_location = __LONG_MAX__;
    for (int sr = 0; sr < num_seed_ranges; ++sr)
    {
        for (long seed = seeds[sr].start; seed < seeds[sr].limit; ++seed)
        {
            #ifdef DISPLAY_PROGRESS
            long percent = ++progress / total;
            if (percent != old_percent)
            {
                printf("%ld%%\n", percent);
                old_percent = percent;
            }
            #endif

            long soil = lookup(seed, seed_to_soil, &seed_to_soil_cache);
            long fertilizer = lookup(soil, soil_to_fertilizer, &soil_to_fertilizer_cache);
            long water = lookup(fertilizer, fertilizer_to_water, &fertilizer_to_water_cache);
            long light = lookup(water, water_to_light, &water_to_light_cache);
            long temperature = lookup(light, light_to_temperature, &light_to_temperature_cache);
            long humidity = lookup(temperature, temperature_to_humidity, &temperature_to_humidity_cache);
            long location = lookup(humidity, humidity_to_location, &humidity_to_location_cache);

            if (location < min_location) min_location = location;
        }
    }

    printf("Result: %ld\n", min_location);

    return 0;
}

static int parse_seeds(char *seed_str)
{
    char *savep;
    char *token = strtok_r(seed_str, " ", &savep);

    int count;
    for (count = 0; token != NULL; ++count)
    {
        seeds[count].start = atol(token);
        token = strtok_r(NULL, " ", &savep);
        seeds[count].limit = seeds[count].start + atol(token);

        token = strtok_r(NULL, " ", &savep);
    }

    return count;
}

static void read_map(FILE *input, struct map_range *mappings)
{
    int count;
    for (count = 0;;++count) {
        long length;
        long dest;
        int nread = fscanf(input, "%ld %ld %ld", &dest, &mappings[count].src, &length);
        if (nread != 3) break;

        /* pre-calculate for more optimal lookups */
        mappings[count].limit = mappings[count].src + length;
        mappings[count].offset = dest - mappings[count].src;
    }
    mappings[count].limit = 0; /* NULL-terminate */
}

static long lookup(long source, struct map_range *mappings, struct map_range *cache)
{
    // try cached first
    if (source >= cache->src && source < cache->limit)
        return source + cache->offset;

    // cache miss, find new mapping
    for (int i = 0; mappings[i].limit != 0; ++i)
    {
        if (source >= mappings[i].src && source < mappings[i].limit)
        {
            /* found: copy to cache, calculate result */
            memcpy(cache, mappings + i, sizeof(struct map_range));
            return source + mappings[i].offset;
        }
    }

    // not found, set cache to a pseudo-mapping item to cover gap where current
    // *source* belongs:
    //  - find largest limit which is smaller (or equal) than *source* - use as src
    //  - find smallest src, that is larger than *source* - use as limit
    //  - the offset should be 0
    cache->offset = 0;
    cache->src = 0;
    cache->limit = __LONG_MAX__;

    for (int i = 0; mappings[i].limit; ++i)
    {
        if (mappings[i].limit <= source && cache->src < mappings[i].limit)
            cache->src = mappings[i].limit;
        if (mappings[i].src > source && cache->limit > mappings[i].src)
            cache->limit = mappings[i].src;
    }

    return source;
}
