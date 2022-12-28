from collections import defaultdict
import re
import copy

def  main(filename, row):
    f = open(filename, "r")
    not_it = set()
    beacons = set()
    for line in f:
        sx, sy, bx, by = map(int, re.findall("-?\d+", line))
        if by == row:
            beacons.add(bx)
        s_range = abs(sx-bx) + abs(sy-by)
        r_dist = abs(sy-row)
        x_left = s_range - r_dist
        if x_left > 0:
            for x in range(sx-x_left, sx+x_left+1):
                not_it.add(x)
    print("p1: " + str(len(not_it - beacons)))
    print("p2: " + "")


if __name__ == "__main__":
    main("input_t1", 10)
    main("input", 2000000)
