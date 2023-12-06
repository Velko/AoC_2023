namespace seeds.tests;

public class SeedsTests
{
    #region SampleInput
    const string SampleInput = @"
seeds: 79 14 55 13

seed-to-soil map:
50 98 2
52 50 48

soil-to-fertilizer map:
0 15 37
37 52 2
39 0 15

fertilizer-to-water map:
49 53 8
0 11 42
42 0 7
57 7 4

water-to-light map:
88 18 7
18 25 70

light-to-temperature map:
45 77 23
81 45 19
68 64 13

temperature-to-humidity map:
0 69 1
1 0 69

humidity-to-location map:
60 56 37
56 93 4
    ";
    #endregion
    
    SeedLookup CreateSeedLookup()
    {
        using var input = new StringReader(SampleInput);
        return new SeedLookup(input);
    }
    [Fact(Skip = "The ultimate integration test")]
    public void SampleInput_should_produce_expected_Location()
    {
        var sut = CreateSeedLookup();

        var result = sut.SearchSmallestLocation();

        Assert.Equal(46, result);
    }

    [Fact]
    public void SeedsShouldBeLoadedAsRanges()
    {
        var sut = CreateSeedLookup();

        Assert.Equal(new[] {
            new SeedRange { Start = 79, Size = 14},
            new SeedRange { Start = 55, Size = 13},
        }, sut.seeds);
    }
}