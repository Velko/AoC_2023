#!/usr/bin/python3

hailstones = []

TEST_AREA_MIN = 200000000000000
TEST_AREA_MAX = 400000000000000

# TEST_AREA_MIN =         7
# TEST_AREA_MAX  =        27

POSITION=0
VELOCITY=1

X=0
Y=1

def compare_hail(a, b):
    dividend_b = (b[POSITION][Y] - a[POSITION][Y]) * a[VELOCITY][X] - (b[POSITION][X] - a[POSITION][X]) * a[VELOCITY][Y];
    divisor_b = b[VELOCITY][X] * a[VELOCITY][Y] - b[VELOCITY][Y] * a[VELOCITY][X]

    dividend_a = (a[POSITION][Y] - b[POSITION][Y]) * b[VELOCITY][X] - (a[POSITION][X] - b[POSITION][X]) * b[VELOCITY][Y];
    divisor_a = a[VELOCITY][X] * b[VELOCITY][Y] - a[VELOCITY][Y] * b[VELOCITY][X]

    if divisor_b == 0:
        return False
    
    if (dividend_b > 0 and divisor_b < 0) or (dividend_b < 0 and divisor_b > 0): # signs differ, intersected in past
        return False
    
    if (dividend_a > 0 and divisor_a < 0) or (dividend_a < 0 and divisor_a > 0): # signs differ, intersected in past
        return False

    intersect_x = b[POSITION][X] + b[VELOCITY][X] * dividend_b / divisor_b
    intersect_y = b[POSITION][Y] + b[VELOCITY][Y] * dividend_b / divisor_b

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