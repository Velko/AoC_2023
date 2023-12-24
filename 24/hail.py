#!/usr/bin/python3

hailstones = []

TEST_AREA_MIN = 200000000000000
TEST_AREA_MAX = 400000000000000

# TEST_AREA_MIN =         7
# TEST_AREA_MAX  =        27


def compare_hail(a, b):
    dividend_b = (b[0][1] - a[0][1]) * a[1][0] - (b[0][0] - a[0][0]) * a[1][1];
    divisor_b = b[1][0] * a[1][1] - b[1][1] * a[1][0]

    dividend_a = (a[0][1] - b[0][1]) * b[1][0] - (a[0][0] - b[0][0]) * b[1][1];
    divisor_a = a[1][0] * b[1][1] - a[1][1] * b[1][0]

    if divisor_b == 0:
        return False
    
    if (dividend_b > 0 and divisor_b < 0) or (dividend_b < 0 and divisor_b > 0): # signs differ, intersected in past
        return False
    
    if (dividend_a > 0 and divisor_a < 0) or (dividend_a < 0 and divisor_a > 0): # signs differ, intersected in past
        return False

    intersect_x = b[0][0] + b[1][0] * dividend_b / divisor_b
    intersect_y = b[0][1] + b[1][1] * dividend_b / divisor_b

    fits = intersect_x >= TEST_AREA_MIN and intersect_x <= TEST_AREA_MAX \
             and intersect_y >= TEST_AREA_MIN and intersect_y <= TEST_AREA_MAX
    
    return fits


with open("input.txt") as f:
    for line in f:
        p, v = line.split("@")
        pos = tuple(map(lambda x: int(x.strip()), p.split(',')))
        vel = tuple(map(lambda x: int(x.strip()), v.split(',')))
        hailstones.append((pos, vel))

total = 0

for i, a in enumerate(hailstones):
    for b in hailstones[i+1:]:
        if compare_hail(a, b):
            total += 1

print (total)