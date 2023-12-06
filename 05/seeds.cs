using System.IO;
using System.Collections.Generic;

struct MapRange
{
    public long Dest;
    public long Src;
    public long Size;
    public long Limit => Src + Size;
    public long DestLimit => Dest + Size;

}

struct SeedRange
{
    public long Start;
    public long Size;
    public long Limit => Start + Size;
}

class Program
{
    static List<SeedRange> seeds = new List<SeedRange>();
    static List<MapRange> seed_to_soil = new List<MapRange>();
    static List<MapRange> soil_to_fertilizer = new List<MapRange>();
    static List<MapRange> fertilizer_to_water = new List<MapRange>();
    static List<MapRange> water_to_light = new List<MapRange>();
    static List<MapRange> light_to_temperature = new List<MapRange>();
    static List<MapRange> temperature_to_humidity = new List<MapRange>();
    static List<MapRange> humidity_to_location = new List<MapRange>();

    static int Main()
    {
        using var input = new StreamReader("input.txt");

        for (;;)
        {
            string? line = input.ReadLine();
            if (line == null) break;

            if (line.StartsWith("seeds: "))
                ParseSeeds(line.Substring(7));
            else if (line.StartsWith("seed-to-soil map:"))
                ReadMap(input, seed_to_soil);
            else if (line.StartsWith("soil-to-fertilizer map:"))
                ReadMap(input, soil_to_fertilizer);
            else if (line.StartsWith("fertilizer-to-water map:"))
                ReadMap(input, fertilizer_to_water);
            else if (line.StartsWith("water-to-light map:"))
                ReadMap(input, water_to_light);
            else if (line.StartsWith("light-to-temperature map:"))
                ReadMap(input, light_to_temperature);
            else if (line.StartsWith("temperature-to-humidity map:"))
                ReadMap(input, temperature_to_humidity);
            else if (line.StartsWith("humidity-to-location map:"))
                ReadMap(input, humidity_to_location);
        }


        long min_location = long.MaxValue;

        foreach (var seed in seeds)
        {
            long soil = Lookup(seed.Start, seed_to_soil);
            long fertilizer = Lookup(soil, soil_to_fertilizer);
            long water = Lookup(fertilizer, fertilizer_to_water);
            long light = Lookup(water, water_to_light);
            long temperature = Lookup(light, light_to_temperature);
            long humidity = Lookup(temperature, temperature_to_humidity);
            long location = Lookup(humidity, humidity_to_location);

            if (location < min_location) min_location = location;
        }

        // sample result: 35
        // real result: 261668924
        Console.WriteLine($"Result: {min_location}");

        return 0;
    }

    static void ParseSeeds(string seed_str)
    {
        var ranges = seed_str
            .Split(" ")
            .Select(long.Parse)
            .ToArray();

        for (int i = 0; i < ranges.Length; i += 1)
        {
            seeds.Add(new SeedRange {
                Start = ranges[i],
                Size = 1,//ranges[i + 1]
            });
        }
    }

    static void ReadMap(StreamReader input, List<MapRange> range)
    {
        int count = 0;

        for (;;++count) {
            string? line = input.ReadLine();
            if (string.IsNullOrEmpty(line)) break;
            var values = line.Split(" ").Select(long.Parse).ToList();

            range.Add(new MapRange
            {
                Dest = values[0],
                Src = values[1],
                Size = values[2],
            });
        }
    }

    static long Lookup(long source, List<MapRange> range)
    {
        foreach (var item in range)
        {
            if (source >= item.Src && source < item.Limit)
                return item.Dest + (source - item.Src);
        }

        return source;
    }

}



