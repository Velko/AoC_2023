Advent of Code 2023
===================

These are my notes about solving the programming puzzles in Advent of Code 2023.

Little craving for oldschool pushed me to pick C language as primary tool. Additional limitations
I'd like to keep is not using external libraries. Still, I do not place hard limitations on that,
could use whatever works to get the result.

This is my 1st time doing Advent of Code, so I'm not exactly sure how it will go.

Started a bit late, had to catch up few days in the beginning.


Day 1
=====

Part 1
------

It started easy, just reading text from a file line by line. Then and check each character if it is a digit.
When first and last digits were found converted them to integers. Here I did not turn to any library function, just
used the fact that in ASCII table digits start from '0' and are sequential up to '9'. Subtracting ASCII code of
'0' from the digit's character gives the numeric value. Multiply the first by 10, add the last. Done!


Part 2
------

For Part 2 I first did small refactoring, extracting the digit detection and conversion separately. As there is no
0 in the input, that value works well as "not found" value.

After that I added the word recognition: instead of looking at single character at the position, I checked if the
beginning of remaining string matches with any word for digits. Move one position forward, repeat. Whichever value
from both digit and word recognition functions is first and last makes it into the result.


Day 2
=====

Here I started to get a feeling that there's some boilerplate code that can be copied from previous puzzle - reading
text line-by line and adding the results of a processing function together.

Part 1
------

The hardest part was to parse the input lines properly. There are no fancy Split and RegEx routines in Standard Library
(I'm sure there are external libs for that, but I'm keeping things simple). Still, sscanf() and strtok_r() can also handle
the task just fine.

After that it was just matter of finding the max number of each cubes and return Id if these matches required criteria.

Part 2
------

Adjusting for Part 2 was quite easy. I could not resist to just slap a _return_ statement multiplying the numbers in the
middle of the function, before starting to check Part 1's criteria.


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

First order of business is again parsing the input, but being armed with experience from Day 2 it did not felt too bad.

The scoring calculation felt a bit awkward until I realized it is more naturally described with bit-shifts (or multiplying
by 2, if you like that better). Let's start with value of 1 and shift it left for each winning number.

0 winning cards get  0b0001 = 1
1 winning card  gets 0b0010 = 2
2 winning cards get  0b0100 = 4
3 winning cards get  0b1000 = 8

This produces the result that is exactly 2 times the expected. How to get the expected result? D'oh! Shift it back right
one place.

Part 2
------

This stirs things up, at first I thought that some kind of priority queue will be required. Working in plain C it is not
an easy thing to implement (15 days later I had to do it anyway). I'm sure there is a lib for that, but I'd like to avoid
that.

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
family, still the standard libraries are completely different. Conversions of the parsing logic took a while.

Now I got an opportunity to check whatever I want. TDD for the rescue!!! Well, yeah, I still got no ideas how to proceed,
so I gave up for a moment and switched to Day 6.


Part 2 - continued
------------------

After successfully brute-forcing Day 6 I was ready to try it out here as well.

First thing: I should create a progress indicator. Calculating the whole seeds range is just adding all the lengths together.

Added a few loops and printing for percentage. When I saw tenths of percents changing every few seconds, it was enough:
I can wait for it. Surely enough, half an hour later I got my result.

Use the Force, Luke!
-- Obi-Wan Kenobi


Part 2 - beyound the solution
-----------------------------

But, you cannot control it. This is a dangerous time for you. When you will be tempted by the dark side of the force.
-- Obi-Wan Kenobi

My brute-force solution was still the the C# one. I wanted to see how quick it will run in pure C. Pulled out the
old Part 1 C file and applied same logic. It ran faster, but slow enough for me not to bother measuring the difference.

I can make it go faster!

I came up with an idea: cache the range just found by a lookup. There's a great chance that I'll be looking at the same
range for next several million iterations. Why not check it first? Also, when the value does not fall in any existing
range, I could probably make up an artificial one, to avoid searching for it all the time.

In the end I managed to reduce the run time of brute-force down to 12.4 seconds (running on almost 7 years old i5-7500).

Is this how the puzzle was _supposed to be solved_? No, but it works, and works fast enough.


Day 6
=====

Part 1
------

The description of the Day 6 described a brute-force algorithm for the calculations. Since the input values were small, I
did not bother with more clever solutions. Just checked all the possibilities.

The input data was also too small for me to bother with parsing. Just hardcoded the values. Looking back, I probably should
had parsed it anyway. I had to do a lot of ``git rebase`` to remove it when I decided that I want to publish the repo.

Part 2
------

Larger numbers suggested that the puzzle was meant to be solved in a different way. I started to think about algebraic solution,
but after arriving at a quadratic equation felt lazy. Probably I just run the loop from Part 1?

Sure enough, the result was ready in a fraction of a second. Good! Now I have an idea for Day 5 part 2.

Part 2 - continued
------------------

After some time I felt that I still need to solve the algebraic version. It's an upside-down parabola (think "rainbow"), and previous
record sets the "horizon". So the result is a difference between the roots of quadratic equation. I came back and rewrote it using
the textbook math.

Had I done this initially, I would probably "poked around" at both roots to get the exact _integer_ values. I do not trust floating-point
operations, but since rounded result was same as I got earlier, I'm happy with that.


Day 7
=====

Part 1
------

I had some experience from Exercism with Poker-style calculations, so the combination detection was not new for me: count the cards
of each value and order the groups descending by their size. Now you can check the numbers of first top groups to determine
the combination.

The rest of the comparison felt a bit off. Instead of trying to look for strongest cards, it has to compare them in the input order.
This was probably the camel-related adjustment.

For sorting I pulled out the Standard Library function qsort(), had to supply it with my own custom comparer functions. I could do
a bubble-sort instead, but I feel fine about using function from LibC. I'm using scanf(), strtok() and others anyway.

Part 2
------

I did not bother with complicated ideas for Joker support. When there were Jokers in hand, I created a copy and replaced J's with all
non-Joker cards in succession.

Day 8
=====

Part 1
------

It was quite straightforward. Parse the input, follow the instructions and count steps. Nothing fancy.

Part 2
------

How hard can it be? Armed with courage from Day 5, I applied some optimizations for speed, prepared straightforward brute-force
solution and let it run. Had to leave the computer for several hours, was hoping to be greeted with a result when I came back. Nope.

Most annoyingly this was a problem where one can not attach a progress indicator to. Should I stop it? What if I'm only 5 seconds
from the result?

Since I had no big experience in solving similar programming puzzles, I headed over to Reddit for hints. Everybody's talking about
a thing called LCM. Refreshed my knowledge on what it is (it has been a long time since school). I thought of that before, but
decided that it would not work. Why would it?

Because, the input is explicitly crafted in a way it would!

A good lesson learned: the input file is also a part of the puzzle.

Then it is just a matter of measuring the length of each cycle and doing the math to obtain the final result.

Part 2 - Use the force, again
-----------------------------

Now that I knew the final answer, I can measure how long would it take to get there by raw computing power.
Made a lot more optimizations and changed the condition of completion. Instead of waiting for all 6 paths to
converge, what about just 4 or 5? 

Turns out, 4 out of 6 paths converge in about 9 seconds. Getting to 5 / 6 takes a little over 10 minutes. The
fastest I managed to achieve was 323,246,164 steps/sec on hardware I have. Quite impressive I think.

How hard then would it be to get to the final result? I know the number that I need to arrive at and it is only
73 times larger that one for the 5 / 6. So a little over 12 hours.

Somebody on Reddit claimed that he got the result in only 6 hours. I know Rust is comparable to C in terms of
speed, but I'm out of optimization ideas. Or maybe he had a faster processor - mine is almost 7 years old already.


Day 9
=====

Part 1
------

This came quite relaxing. Especially after couple of harder puzzles that week. Just parse the input, fill the array
and iterate a bit forth and back.

Part 2
------

Was expecting some more complexity. Not this time. I think it even requires one less variable than Part 1.


Day 10
======

Part 1
------

So, this time I have to find out the starting direction from and follow the route around until I complete the loop. The farthest
point must be a half of the distance, rounded up. In fact I forgot about the rounding up, but was lucky because the total distance
turned out even.


Part 2
------

First thing of the business, I should get rid of that junk pipe. I decided to walk around the main loop and copy it over to
a fresh ground. Then it would contain only parts I need to pay attention to.

Now, how do I distinguish *inside* from *outside*? First thing that came in mind: if I'm walking counter-clockwise, then the
*inside* should always be *to the left* from me. Then I can walk around again and mark every empty ground tile with *I*.

When done with that, it is just a matter to run a *flood fill* algorithm over it and count the results.

Did not go as smooth as I wanted to, however. I made the most stupid mistake here - forgot a return statement. Up until now
I was using just GCC compiler, without any specific flags. Turns out it does not report any warnings, until they are turned
on explicitly.

I was banging my head against the keyboard and implementing a helper routines to figure out what was wrong.

Everything was Ok to begin with, except for that stupid *return*. Anyway, added a template Makefile with *-Wall -Wextra*.

Still the one of my helpers: *pretty printing* turned out quite nice, much easier to follow. I replaced the original
L-7|F (and other) characters with nice block drawing ones: └ ─ ┐ │ ┌. Oh, the good old DOS days.


Day 11
======

Part 1
------

Started by loading the input. Allocated 2D array twice as big, as empty space could expand that much. Then the expansion itself,
started by extending columns - copied the remainder of the each row forward one byte using memmove() at each column that needed
expansion. Then did the same for rows, the only difference that now I could copy whole array forward, again using memmove(). Doing
columns first meant that there were less looping, but it would have worked in the opposite order as well.

The distance turned out to be Manhattan distance, so no mocking around with Pythagorean theorem calculations and square roots. Just
add together the coordinate differences. Started to think about Dijkstra algorithm until realize that there are way too much "pairs"
mentioned in the example, and it actually means all distances between each galaxies.

Extracted the coordinates of each galaxy into an array and looped through it. Done!


Part 2
------

Ok, expanding each empty row and column million times would not have worked. The array would take 17.4 petabytes, for completely
empty space. Even using virtual memory tricks on x86_64 would fall short.

Obviously, the space should be expanded virtually. Filled 2 arrays, one to hold virtual coordinates for column, another for row:
sort of a rulers for measurement. If a column contains at least one galaxy, the tick increases by just 1, but if there' none,
it increments by a million. No, actually by only 999,999.

The rest was just searching for galaxies and looking up the coordinates when found. That and changing data types to *long*.

The virtual expansion had a big for off-by-one error. Fortunately I first tested it if I get the same result as in Part 1 using
expansion factor of 2.


Day 12
======

Part1
-----

For this one I went for a simple brute-force solution. I knew it will probably not work for second part, but whatever, will think
about it when I'll get there.

Checked that there are maximum 19 question marks, let's do a loop for each combination ranging from 0 to
2^number_of_question_marks. Replace each ? with . or # depending if corresponding bit is 0 or 1. Relatively
straightforward.

Struggled a bit with comparing the reconstructed line with the group numbers, until I realized that I can parse
it using strtok() and measure the string length.

Part 2
------

Ok this will be a tough one! Looping from 0 to 2^19 (524288) is nothing. Looping up to 2^99 is whole another story,
I need a better algorithm.

First, however, I need a strong test base to test my algorithm against. I modified the program to output original
input lines, each amended with per-line result. Then adjusted the parser to also extract that and check if processing
returns the anticipated number.

The new algorithm was: take the input template and try to fill it with # and . according to each group counts. Once
a valid solution for the group is found, recurse for the next group. Once reached end of template or group, return
back if it matches or not.

When scanning the template, there are 4 possible options:
    * found '.' - just scan further
    * found '#' - make a "hard match" if the group can be placed and subsequent recursion is successful
    * found '?' - make a "soft match" - try to place a group, but failing to do that just continue to next
    * reached end of template - return what's collected so far

When there are no more groups to check, one should check if the end of template requires any '#', or the remainder
can be filled by '.'. Former means failure, latter - successful match.

It took a lot of debugging to get everything right, but at the end I got it to the point where it was able to solve
Part 1 successfully again. Could not had done it without the test-cases I generated earlier.

Now it should be able to solve Part 2 as well? Added the "unfolding" logic, ran it. First few lines went fine and
then it got stuck. Is there a bug? Or it's just a complicated case and takes a while?

After had checked everything again, I did not find any possible infinite loops, it must be complicated case. For that
line there was 64 solutions in Part 1, so it must be pretty complicated. Should I just wait or think of better solution?

Headed over to Reddit and saw that everybody talks about technique called memoization, decided to check out what that
is about. Given that nothing else changes, if you call a function with same parameters, it should return same result.

What do I pass to my recursive function? Reconstructed string, index where to continue and group. Do I need to hash
them all together and use as a key? In C? Seriously?

Wait, do I need to pass the reconstructed string, I'm not looking at any position smaller than the starting index.
Can I just use the original template? Changed - still works. Hey, now I'm building the reconstructed string but
never actually read anything from it. I must be able to eliminate that.

The reconstructed string was an invaluable resource while I was debugging the algorithm, but now it's time to say
goodbye. Removed, everything still works.

Now I', passing in just a constant string, starting index and group. This looks more memo-izable. I created a 2D array
addressable by starting indices and group indices. Fill it with -1 and call the function only when there's no result
from previous call.

This time it finished instantly.


Day 13
======

Part 1
------

After loading the input into array of strings, implemented a row-by-row mirror scanning. It's easier, because I can use
standard strcmp() function for that. What about columns? Felt lazy and wrote routine to transpose the array. Now the columns
are converted to rows and I can reuse same row-by-row scanner.

Part 2
------

The amount of data is quite small, I decided to go for a brute-force. Flip each cell and check with the previous function.

Had to do bit of head-scratching, because sometimes it could not found the solution. Turns out - if I flip a cell that does
not impact the original solution, can still get the original back. Some proper "skip" logic is in order.

Part 2 - continued
------------------

Some time later I realized that I could replace the standard _strcmp()_ with my own _smudgecmp()_ that returns the number of
differences found. Now, if I allow at most 1 difference per mirror, it gives me the expected results.

Day 14
======

Part 1
------

The solution felt obvious, just model the movement of the rocks and calculate the result.

Part 2
------

I have to model the movements when platform is tilted elsewhere. There must be a point when movement of the platform returns to
a state it had before. Then I should be able to skip the repeating cycles and drop right into the final segment of the cycles.

I decided to pack the state of the platform into a smaller representation, where the rock is represented by binary 1 and everything
else with a 0. This reduced my memory consumption 8 times, but probably I could had done without it anyway. When I later peeked at
solutions on Reddit, they were using some hashing instead. Still my version works.

Once cycle detection found a state where system has been before, it was a simple matter to skip all the unnecessary cycles and drop
into the last leg of the iterations. Once finished, just calculate the result same way as in Part 1.

Day 15
======

Part 1
------

Since I'm using C language, some of the calculation requirements came naturally:
* ASCII code _is_ the integer value of character
* remainder of dividing it by 256 means just storing it into a 8-bit variable

So the calculations were obvious.

Part 2
------

It was just matter of following the instructions and adjusting the state of the lenses accordingly. That and calculating the result at the end.

Day 16
======

Part 1
------

The hard part was splitting of the beam. Until it occurred to me that there's no point of following the beam that has already been at the location,
moving in the same direction. The tile was energized once, there's nothing new the beam can do.

Part 2
------

Starting the beam from any edge of the floor? Does this even counts as a brute-forcing if I'm checking 440 possibilities?

Day 17
======

Part 1
------

The easy days are over! A path-finding puzzle has arrived. I was thinking a Dijkstra's algorithm is in order, but those 3 straight steps at most
threw me off course. I was in a middle of re-inventing the Dijkstra's algorithm (from what I remembered from my university courses), when I took
a peek at Reddit. Ok, it's Dijkstra, but there's some complex state. 

And now I have to implement a _priority queue_. Implemented dumbest thing possible: _memmove()_ after each pop and _qsort()_ after each insert, but it worked (albeit was not as quick as I'd like to) and I got my result.

Funny, the Dijkstra's algorithm was nothing similar to one I remembered from university, but here I have it, at more general form.

Part 2
------

This did not make much complications. Just add the minimum straight steps and change the turn limit.

Day 18
======


Part 1
------

The obvious thing was to just follow the digging instructions. It, however, was not clear at which coordinate start the dig. It can be solved by
following the path once, just calculating the coordinates and recording the min/max ones.

Then start the dig again, but start at the coordinates that leaves 1 step border around the trench. Now, do the floodfill from (0, 0) and everything
not filled belongs to the pit.

Part 2
------

The initial idea was to use similar approach as on Day 11 and use virtual coordinates. It would create a ratio-map of what each cell is worth. The
idea felt sound, but I struggled to implement it properly.

At some point I peeked at Reddit and saw the solution using _Shoelace formula_ along with _Pick's theorem_ as the common solution. Implemented that and
got my answer. Still, I leaned new things.

Part 2 - continued
------------------

I, however, did not feel satisfied and felt that my original idea should had also worked. Turned out I was close, but starting to move in wrong direction.
Sleeping it over helped, and I was able to implement it successfully. Later I learned that the technique is called _Coordinate compression_.

