Advent of Code 2023
===================

These are my notes about solving the programming puzzles in Advent of Code 2023.

Little craving for oldschool pushed me to pick C language as primary tool. Additional limitations
I'd like to keep is not using external libraries. Still, I do not place hard limitations on that,
could use whatever works to get the result.

I started a bit late, had to catch up few days in the beginning.


Day 1
=====

Part 1
------

It started easy, just reading text from a file line by line. Then and check each character in it if is a digit.
When first and last digits were found convert them to integer. Here I did not turn to any library function, just
used the fact that in ASCII table digits start from '0' and are sequential up to '9'. Subtracting ASCII code of
'0' from the digit's character gives the numeric value. Multiply the first by 10, add the last. Done!


Part 2
------

For part 2 I first did small refactoring, extracting the digit detection and conversion separately. As there is no
0 in the input, that value works well as "not found" value.

After that I added the word recognition: instead of looking at single character at the position, I checked if the
beginning of remaining string matches with any word for digits. Move one position forward, repeat. Whichever value
from both digit and word recognition functions is first and last is the result.


Day 2
=====

Here I started to get a feeling that there's some boilerplate code that can be copied from previous puzzle - reading
text line-by line and adding the results of a processing function together.

Part 1
------

The hardest part was to parse the input lines properly. There are no fancy Split and RegEx routines in standard library
(I'm sure there are libs for that, but I'm keeping things simple), but sscanf() and strtok_r() can also handle the task.

After that it was just matter of finding the max number of each cubes and return Id if these matches required criteria.

Part 2
------

Part 2 was even easier, one does not have to check any criteria, just multiply the numbers.


Day 3
=====

Part 1
------

The basic idea for solution was quite straightforward: load whole input in 2D array, look for symbols. When found, check
all 8 adjacent cells around, if they contain digits.

Then scan the line a bit back and forward for additional digits and extract the value.

There was an issue, however, how to avoid a number that is placed directly above (or below) the symbol from being detected
multiple times. Solution was to look at the location directly above (or below) first, and check diagonals in that direction
only if it is empty.

Part 2
------

Now I had to look only for specific symbol '*' and switch the operation from adding all numbers together to counting and
multiplying them. Check if count is 2 and only then add it to the total.


Day 4
=====

At this point I definitely felt, that I'm copying same code from previous puzzles all the time, so I decided to set it
aside as a template/boilerplate project, that I can always use as a starting point.

Part 1
------

First order of business is again parsing the input, but being armed with experiece from Day 2 it did not felt too bad.

The scoring calculation felt a bit awkward until I realized that bit-shifts is what describes it naturally. Let's start
with value of 1 and shift it left for each winning number.

0 winning cards get  0b0001 = 1
1 winning card  gets 0b0010 = 2
2 winning cards get  0b0100 = 4
3 winning cards get  0b1000 = 8

This produces the result that is exactly 2 times the expected. How to get the result? D'oh! Just shift it right one place.

Part 2
------

This stirs things up, at first I thought that some kind of priority queue will be required. Working in plain C it is not
an easy thing to implement. I'm sure there is a lib for that, but I'd like to avoid that.

Thinking further, the additional copies is just a number. There is no need to create additional "card objects" that are
identical to the original. Start with the count of 1 and add whatever additional copies were won.


Day 5
=====

Part 1
------

Again, there's considerable effort in parsing the input. 7 levels is a bit too much I'm willing to go manually, but it
felt like a good idea, so I did it anyway. I probably could have made some sort of loop, but it is what it is.

After that it's looking up each seed through all 7 layers and picking the lowest.

Part 2
------

The meme about 4 nice doggies and a werewolf was quite accurate. This was the first time I had ran out of ideas. Surely,
there was thoughts about looking up the limits in next level and then splitting up the highest one. Or trying to traverse
the mapping in reverse.

One thing was clear, this definitely will require some dynamically allocated data structures for the unknown number of new
ranges. Then I'll need a way to check out different scenarios. Complicated thing to pull off in plain C. At this point
I decided that I need sharper tools (pun intended) and decided to port everything to C#. They belong to the same language
family, still the standard libraries are completely different. The parsing logic took a while.

Now I got an opportunity to check whatever I want. TDD for the rescue!!! Well, yeah, I still got no ideas how to proceed,
so I gave up switched to Day 6.


Part 2 - continued
------------------

After successfully brute-forcing Day 6 I was ready to try it out here as well.

First thing: I should create a progress indicator. Calculating the whole seeds range is just adding the lengths together.

Added a few loops and printing for percentage. When I saw tenths of percents changing every few seconds, it was enough:
I can wait for it. Surely enough, half an hour later I got my result.

Use the Force, Luke!
-- Obi-Wan Kenobi


Part 2 - beyound the solution
-----------------------------

But, you cannot control it. This is a dangerous time for you. When you will be tempted by the dark side of the force. 
-- Obi-Wan Kenobi

My brute-force solution was still in the C# solution. I wanted to see how quick it will run in pure C. Pulled out the
old Part 1 C file and applied same logic. It ran faster, but slow enough for me not to bother measuring the difference.

I can make it go faster!

I came up with an idea: cache the range just found by a lookup. There's a great chance that I'll be looking at the same
range for next several million iterations. Why not check it first? Also, when the value does not fall in any existing
range, I could probably make up an artificial one, to avoid looking for it all the time.

In the end I managed to reduce the run time of brute-force down to 12.4 seconds (running on almost 7 years old i5-7500).


Day 6
=====

Part 1
------

The description of the Day 6 described a brute-force algorithm for the calculations. Since the input values were small, I 
did not bother with more clever solutions. Just checked all the possibilities.

The input data was also too small for me to bother with parsing. Just hardcoded the values.

Part 2
------

Larger numbers suggested that the puzzle was meant to be solved in a different way. I started to think about algebraeic solution,
but after arriving at a quadratic equation felt lazy. Probably I just run the loop from Part 1?

Sure enough, the result was ready in a fraction of a second. Good! Now I have an idea for Day 5 part 2.



Day 7
=====

Part 1
------

New day, new challenge. Parsing part was not too complicated.

I had some experience from Exercism with Poker-style calculations, so the combination detection was not new for me: count the cards
of each value and order the result descending by the number. Now you can check the numbers of first top records to determine
the combination.

The rest of the comparison felt a bit off. Instead of trying to look for strongest cards, it has to compare them in the input order.

For sorting I pulled out the Standard Library function qsort(), had to supply it with my own custom comparer functions. I could do
a bubble-sort instead, but I feel fine about using function from LibC. I'm using scanf(), strtok() and others anyway.

Part 2
------

I did not bother with complicated ideas for Joker support. When there were Jokers in hand, I created a copy and replaced J's with all
non-Joker cards in succession.



