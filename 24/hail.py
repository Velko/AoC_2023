#!/usr/bin/python3

import z3

hailstones = []

TEST_AREA_MIN = 200000000000000
TEST_AREA_MAX = 400000000000000

# TEST_AREA_MIN =         7
# TEST_AREA_MAX  =        27

POSITION=0
VELOCITY=1

X=0
Y=1


def gauss_eliminate(matrix):

    nrows = len(matrix)
    ncols = nrows + 1

    for i in range(nrows):
        d = matrix[i][i]
        if d == 0: return False
        for c in range(i, ncols):
            matrix[i][c] /= d

        for j in range(i + 1, nrows):
            m = matrix[j][i]
            for c in range(i, ncols):
                matrix[j][c] -= matrix[i][c] * m

    for i in range(nrows - 1, 0, -1):
        for j in range(i):
            m = matrix[j][i]
            for c in range(i, ncols):
                matrix[j][c] -= matrix[i][c] * m

    return True



def compare_hail(a, b):

    matrix = [[a[VELOCITY][X], -b[VELOCITY][X],b[POSITION][X] - a[POSITION][X]],
              [a[VELOCITY][Y], -b[VELOCITY][Y],b[POSITION][Y] - a[POSITION][Y]]]

    if not gauss_eliminate(matrix):
        return False

    time_a = matrix[0][-1]
    time_b = matrix[1][-1]

    if time_b < 0:
        return False

    if time_a < 0:
        return False

    intersect_x = b[POSITION][X] + b[VELOCITY][X] * time_b
    intersect_y = b[POSITION][Y] + b[VELOCITY][Y] * time_b

    fits = intersect_x >= TEST_AREA_MIN and intersect_x <= TEST_AREA_MAX \
       and intersect_y >= TEST_AREA_MIN and intersect_y <= TEST_AREA_MAX

    return fits


with open("input.txt") as f:
    for line in f:
        p, v = line.split("@")
        pos = tuple(map(lambda x: int(x.strip()), p.split(',')))
        vel = tuple(map(lambda x: int(x.strip()), v.split(',')))
        hailstones.append((pos, vel))

result1 = 0

for i, a in enumerate(hailstones):
    for b in hailstones[i+1:]:
        if compare_hail(a, b):
            result1 += 1

print (f"Result p1: {result1}")


(xa0, ya0, za0), (vax, vay, vaz) = hailstones[0]
(xb0, yb0, zb0), (vbx, vby, vbz) = hailstones[1]
(xc0, yc0, zc0), (vcx, vcy, vcz) = hailstones[2]

ta, tb, tc, vrx, vry, vrz, xr0, yr0, zr0 = z3.Reals("ta tb tc vrx vry vrz xr0 yr0 zr0")

equations = [
    xr0 + vrx * ta == xa0 + vax * ta,
    yr0 + vry * ta == ya0 + vay * ta,
    zr0 + vrz * ta == za0 + vaz * ta,

    xr0 + vrx * tb == xb0 + vbx * tb,
    yr0 + vry * tb == yb0 + vby * tb,
    zr0 + vrz * tb == zb0 + vbz * tb,

    xr0 + vrx * tc == xc0 + vcx * tc,
    yr0 + vry * tc == yc0 + vcy * tc,
    zr0 + vrz * tc == zc0 + vcz * tc,
]

solver = z3.Solver()
solver.add(*equations)
solver.check()
m = solver.model();

result2 = m[xr0].as_long() + m[yr0].as_long() + m[zr0].as_long()
print (f"Result p2: {result2}")
