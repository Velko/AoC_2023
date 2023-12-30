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

def solve_p2_z3():
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

    print (m[xr0].as_long(), m[yr0].as_long(), m[zr0].as_long())
    print (m[vrx].as_long(), m[vry].as_long(), m[vrz].as_long())

    result2 = m[xr0].as_long() + m[yr0].as_long() + m[zr0].as_long()

    print (f"Result p2: {result2}")

def solve_p2_gauss():
    (xa0, ya0, za0), (vax, vay, vaz) = hailstones[0]
    (xb0, yb0, zb0), (vbx, vby, vbz) = hailstones[1]
    (xc0, yc0, zc0), (vcx, vcy, vcz) = hailstones[2]
    (xd0, yd0, zd0), (vdx, vdy, vdz) = hailstones[3]
    (xe0, ye0, ze0), (vex, vey, vez) = hailstones[4]


    # let's do the calculations around all combinations of axis and average them to
    # improve accuracy
    matrix_xy = [
        [vby - vay, vax - vbx, ya0 - yb0, xb0 - xa0, xb0 * vby - yb0 * vbx - xa0 * vay + ya0 * vax],
        [vcy - vay, vax - vcx, ya0 - yc0, xc0 - xa0, xc0 * vcy - yc0 * vcx - xa0 * vay + ya0 * vax],
        [vdy - vay, vax - vdx, ya0 - yd0, xd0 - xa0, xd0 * vdy - yd0 * vdx - xa0 * vay + ya0 * vax],
        [vey - vay, vax - vex, ya0 - ye0, xe0 - xa0, xe0 * vey - ye0 * vex - xa0 * vay + ya0 * vax],
    ]

    matrix_xz = [
        [vbz - vaz, vax - vbx, za0 - zb0, xb0 - xa0, xb0 * vbz - zb0 * vbx - xa0 * vaz + za0 * vax],
        [vcz - vaz, vax - vcx, za0 - zc0, xc0 - xa0, xc0 * vcz - zc0 * vcx - xa0 * vaz + za0 * vax],
        [vdz - vaz, vax - vdx, za0 - zd0, xd0 - xa0, xd0 * vdz - zd0 * vdx - xa0 * vaz + za0 * vax],
        [vez - vaz, vax - vex, za0 - ze0, xe0 - xa0, xe0 * vez - ze0 * vex - xa0 * vaz + za0 * vax],
    ]

    matrix_yz = [
        [vbz - vaz, vay - vby, za0 - zb0, yb0 - ya0, yb0 * vbz - zb0 * vby - ya0 * vaz + za0 * vay],
        [vcz - vaz, vay - vcy, za0 - zc0, yc0 - ya0, yc0 * vcz - zc0 * vcy - ya0 * vaz + za0 * vay],
        [vdz - vaz, vay - vdy, za0 - zd0, yd0 - ya0, yd0 * vdz - zd0 * vdy - ya0 * vaz + za0 * vay],
        [vez - vaz, vay - vey, za0 - ze0, ye0 - ya0, ye0 * vez - ze0 * vey - ya0 * vaz + za0 * vay],
    ]

    if not gauss_eliminate(matrix_xy):
        raise Exception("WTF?")

    if not gauss_eliminate(matrix_xz):
        raise Exception("WTF?")

    if not gauss_eliminate(matrix_yz):
        raise Exception("WTF?")

    xr0 = round((matrix_xy[0][-1] + matrix_xz[0][-1])/2)
    yr0 = round((matrix_xy[1][-1] + matrix_yz[0][-1])/2)
    vrx = round((matrix_xy[2][-1] + matrix_xz[2][-1])/2)
    vry = round((matrix_yz[3][-1] + matrix_yz[2][-1])/2)
    zr0 = round((matrix_yz[1][-1] + matrix_xz[1][-1])/2)
    vrz = round((matrix_yz[3][-1] + matrix_xz[3][-1])/2)

    print (xr0, yr0, zr0)
    print (vrx, vry, vrz)

    result2 = xr0 + yr0 + zr0
    print (f"Result p2: {result2}")




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

solve_p2_z3()
solve_p2_gauss()

