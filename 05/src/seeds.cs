using System.IO;
using System.Collections.Generic;

public struct MapRange
{
    public long Dest;
    public long Src;
    public long Size;
    public long Limit => Src + Size;
    public long DestLimit => Dest + Size;
    public long Offset => Dest - Src;

}

public struct SeedRange
{
    public long Start;
    public long Size;
    public long Limit => Start + Size;
}

class Program
{
    static void Main()
    {
        using var input = new StreamReader("input.txt");

        var lookup = new SeedLookup(input);


        var min_location = lookup.SearchSmallestLocation();

        // sample result: 35
        // real result: 261668924
        Console.WriteLine($"Result: {min_location}");
    }
}

public class SeedLookup
{
    public readonly List<SeedRange> seeds;
    List<MapRange> seed_to_soil;
    List<MapRange> soil_to_fertilizer;
    List<MapRange> fertilizer_to_water;
    List<MapRange> water_to_light;
    List<MapRange> light_to_temperature;
    List<MapRange> temperature_to_humidity;
    List<MapRange> humidity_to_location;

    public SeedLookup(TextReader input)
    {
        for (;;)
        {
            string? line = input.ReadLine();
            if (line == null) break;

            if (line.StartsWith("seeds: "))
                seeds = ParseSeeds(line.Substring(7));
            else if (line.StartsWith("seed-to-soil map:"))
                seed_to_soil = ReadMap(input);
            else if (line.StartsWith("soil-to-fertilizer map:"))
                soil_to_fertilizer = ReadMap(input);
            else if (line.StartsWith("fertilizer-to-water map:"))
                fertilizer_to_water = ReadMap(input);
            else if (line.StartsWith("water-to-light map:"))
                water_to_light = ReadMap(input);
            else if (line.StartsWith("light-to-temperature map:"))
                light_to_temperature = ReadMap(input);
            else if (line.StartsWith("temperature-to-humidity map:"))
                temperature_to_humidity = ReadMap(input);
            else if (line.StartsWith("humidity-to-location map:"))
                humidity_to_location = ReadMap(input);
        }

        /* to make compiler happy */
        if (seeds == null) seeds = new();
        if (seed_to_soil == null) seed_to_soil = new();
        if (soil_to_fertilizer == null) soil_to_fertilizer = new();
        if (fertilizer_to_water == null) fertilizer_to_water = new();
        if (water_to_light == null) water_to_light = new();
        if (light_to_temperature == null) light_to_temperature = new();
        if (temperature_to_humidity == null) temperature_to_humidity = new();
        if (humidity_to_location == null) humidity_to_location = new();
    }

    public long SearchSmallestLocation()
    {
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

        return min_location;
    }

    static List<SeedRange> ParseSeeds(string seed_str)
    {
        var ranges = seed_str
            .Split(" ")
            .Select(long.Parse)
            .ToArray();

        List<SeedRange> seeds = new List<SeedRange>();
        for (int i = 0; i < ranges.Length; i += 2)
        {
            seeds.Add(new SeedRange {
                Start = ranges[i],
                Size = ranges[i + 1]
            });
        }

        return seeds;
    }

    static List<MapRange> ReadMap(TextReader input)
    {
        List<MapRange> range = new List<MapRange>();

        int count = 0;

        for (;;++count) {
            string? line = input.ReadLine();
            if (string.IsNullOrWhiteSpace(line)) break;
            var values = line.Split(" ").Select(long.Parse).ToList();

            range.Add(new MapRange
            {
                Dest = values[0],
                Src = values[1],
                Size = values[2],
            });
        }

        return FillGaps(range);
    }

    static List<MapRange> FillGaps(IEnumerable<MapRange> range)
    {
        List<MapRange> target = new List<MapRange>();
        long prev_end = 0;
        foreach (var r in range.OrderBy(x => x.Src))
        {
            if (r.Src > prev_end)
                target.Add(new MapRange {
                    Src = prev_end,
                    Dest = prev_end,
                    Size = r.Src - prev_end,
                });
            else if (r.Src < prev_end)
                throw new InvalidOperationException("Range overlap");
            target.Add(r);
            prev_end = r.Limit;
        }

        target.Add(new MapRange {
            Src = prev_end,
            Dest = prev_end,
            Size = long.MaxValue - prev_end,
        });

        return target;
    }

    static long Lookup(long source, List<MapRange> range)
    {
        foreach (var item in range)
        {
            if (source >= item.Src && source < item.Limit)
                return source + item.Offset;
        }

        return source;
    }
}



