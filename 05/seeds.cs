using System.IO;
using System.Collections.Generic;

struct map_range
{
    public long dest;
    public long src;
    public long limit;
};

class Program
{
    static List<long> seeds = new List<long>();
    static List<map_range> seed_to_soil = new List<map_range>();
    static List<map_range> soil_to_fertilizer = new List<map_range>();
    static List<map_range> fertilizer_to_water = new List<map_range>();
    static List<map_range> water_to_light = new List<map_range>();
    static List<map_range> light_to_temperature = new List<map_range>();
    static List<map_range> temperature_to_humidity = new List<map_range>();
    static List<map_range> humidity_to_location = new List<map_range>();

    static int Main()
    {
        using var input = new StreamReader("input.txt");

        for (;;)
        {
            string? line = input.ReadLine();
            if (line == null) break;

            if (line.StartsWith("seeds: "))
                parse_seeds(line.Substring(7));
            else if (line.StartsWith("seed-to-soil map:"))
                read_map(input, seed_to_soil);
            else if (line.StartsWith("soil-to-fertilizer map:"))
                read_map(input, soil_to_fertilizer);
            else if (line.StartsWith("fertilizer-to-water map:"))
                read_map(input, fertilizer_to_water);
            else if (line.StartsWith("water-to-light map:"))
                read_map(input, water_to_light);
            else if (line.StartsWith("light-to-temperature map:"))
                read_map(input, light_to_temperature);
            else if (line.StartsWith("temperature-to-humidity map:"))
                read_map(input, temperature_to_humidity);
            else if (line.StartsWith("humidity-to-location map:"))
                read_map(input, humidity_to_location);
        }


        long min_location = long.MaxValue;

        foreach (var seed in seeds)
        {
            long soil = lookup(seed, seed_to_soil);
            long fertilizer = lookup(soil, soil_to_fertilizer);
            long water = lookup(fertilizer, fertilizer_to_water);
            long light = lookup(water, water_to_light);
            long temperature = lookup(light, light_to_temperature);
            long humidity = lookup(temperature, temperature_to_humidity);
            long location = lookup(humidity, humidity_to_location);

            if (location < min_location) min_location = location;
        }

        // sample result: 35
        // real result: 261668924
        Console.WriteLine($"Result: {min_location}");

        return 0;
    }

    static void parse_seeds(string seed_str)
    {
        seeds.AddRange(seed_str.Split(" ").Select(long.Parse));
    }

    static void read_map(StreamReader input, List<map_range> range)
    {
        int count = 0;

        for (;;++count) {
            string? line = input.ReadLine();
            if (string.IsNullOrEmpty(line)) break;
            var values = line.Split(" ").Select(long.Parse).ToList();

            range.Add(new map_range
            {
                dest = values[0],
                src = values[1],
                limit = values[1] + values[2],
            });
        }
    }

    static long lookup(long source, List<map_range> range)
    {
        foreach (var item in range)
        {
            if (source >= item.src && source < item.limit)
                return item.dest + (source - item.src);
        }

        return source;
    }

}



