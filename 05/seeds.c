#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE     256
#define MAX_RANGES      200  /* there are ~220 lines total in input, should be enough */

struct map_range
{
    long dest;
    long src;
    long limit;
};

struct map_range seed_to_soil[MAX_RANGES];
struct map_range soil_to_fertilizer[MAX_RANGES];
struct map_range fertilizer_to_water[MAX_RANGES];
struct map_range water_to_light[MAX_RANGES];
struct map_range light_to_temperature[MAX_RANGES];
struct map_range temperature_to_humidity[MAX_RANGES];
struct map_range humidity_to_location[MAX_RANGES];

static void parse_seeds(char *seeds);
static void read_map(FILE *input, struct map_range *range);

int main(void)
{
    char line[BUFFER_SIZE];
    FILE *input = fopen("input.txt", "r");

    for (;;)
    {
        if (fgets(line, BUFFER_SIZE, input) == NULL) break; 

        if (strstr(line, "seeds: ") == line)
            parse_seeds(line + 7);
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
    return 0;
}

static void parse_seeds(char *seeds)
{
    printf("S: %s", seeds);
}

static void read_map(FILE *input, struct map_range *range)
{
    int count = 0;
    long length;

    for (;;++count) {
        int nread = fscanf(input, "%ld %ld %ld", &range[count].dest, &range[count].src, &length);
        if (nread != 3) break;
        range[count].limit = range[count].src + length;

        printf("[%ld:%ld) -> %ld\n", range[count].src, range[count].limit, range[count].dest);
    }
    range[count].limit = 0;
}
