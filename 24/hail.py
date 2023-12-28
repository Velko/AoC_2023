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


def gauss_eliminate(matrix):

    # for h in range(len(matrix)): print(matrix[h])
    # print ("-i")

    nrows = len(matrix)
    ncols = nrows + 1

    for i in range(nrows):
        d = matrix[i][i]
        if d == 0: return False
        for c in range(i, ncols):
            matrix[i][c] /= d

        # for h in range(len(matrix)): print(matrix[h])
        # print ("-d")

        for j in range(i + 1, nrows):
            m = matrix[j][i]
            for c in range(i, ncols):
                matrix[j][c] -= matrix[i][c] * m

            # for h in range(len(matrix)): print(matrix[h])
            # print ("-m")

    for i in range(nrows - 1, 0, -1):
        for j in range(i):
            m = matrix[j][i]
            for c in range(i, ncols):
                matrix[j][c] -= matrix[i][c] * m
            # for h in range(len(matrix)): print(matrix[h])
            # print ("-e")
    return True



def compare_hail(a, b):

    matrix = [[a[VELOCITY][X], -b[VELOCITY][X],b[POSITION][X] - a[POSITION][X]],
              [a[VELOCITY][Y], -b[VELOCITY][Y],b[POSITION][Y] - a[POSITION][Y]]]

    if not gauss_eliminate(matrix):
#        print("Parallel")
        return False

#    print("--------------------")
    time_a = matrix[0][-1]
    time_b = matrix[1][-1]

    if time_b < 0:
#        print("Past B")
        return False

    if time_a < 0:
#        print("Past A")
        return False

    intersect_x = b[POSITION][X] + b[VELOCITY][X] * time_b
    intersect_y = b[POSITION][Y] + b[VELOCITY][Y] * time_b

#    print (f"Intersect: {intersect_x}, {intersect_y}")

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

# testm = [[1, 3, -2, 5],
#             [3, 5,  6, 7],
#             [2, 4,  3, 8]]


# gauss_eliminate(testm)

#compare_hail(hailstones[0], hailstones[1])

for i, a in enumerate(hailstones):
    for b in hailstones[i+1:]:
        if compare_hail(a, b):
            total += 1

print (total)