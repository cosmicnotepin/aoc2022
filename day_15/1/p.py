from collections import defaultdict
import re
import copy

def main(filename, row):
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

def main2(filename, bound):
    f = open(filename, "r")
    srs = []
    beacons = set()
    for line in f:
        sx, sy, bx, by = map(int, re.findall("-?\d+", line))
        beacons.add((bx,by))
        s_range = abs(sx-bx) + abs(sy-by) + 1
        s_border = set()
        for x in range(-s_range, s_range + 1):
            s_border.add((sx + x, sy - (s_range - abs(x))))
            s_border.add((sx + x, sy + (s_range - abs(x))))
        srs.append([sx,sy,s_range,s_border])
    of_inter = set()
    for i,s1 in enumerate(srs):
        for j,s2 in enumerate(srs[i:]):
            if abs(s1[0]-s2[0]) + abs(s1[1]-s2[1]) > s1[2] + s2[2]:
                continue
            for k,s3 in enumerate(srs[j:]):
                if abs(s1[0]-s3[0]) + abs(s1[1]-s3[1]) > s1[2] + s3[2]:
                    continue
                if abs(s2[0]-s3[0]) + abs(s2[1]-s3[1]) > s2[2] + s3[2]:
                    continue
                for l,s4 in enumerate(srs[k:]):
                    if abs(s1[0]-s4[0]) + abs(s1[1]-s4[1]) > s1[2] + s4[2]:
                        continue
                    if abs(s2[0]-s4[0]) + abs(s2[1]-s4[1]) > s2[2] + s4[2]:
                        continue
                    if abs(s3[0]-s4[0]) + abs(s3[1]-s4[1]) > s3[2] + s4[2]:
                        continue
                    inter = s1[3] & s2[3] & s3[3] & s4[3]
                    if len(inter) == 1:
                        of_inter |= inter
    for x,y in of_inter:
        seen = False
        for sx,sy,sr,b in srs:
            if abs(sx-x) + abs(sy-y) < sr:
                seen = True
                break
        if seen:
            continue
        elif x>0 and y>0 and x<bound and y < bound:
            print(x*4000000+y)
            break



if __name__ == "__main__":
    #main("input_t1", 10)
    #main("input", 2000000)
    main2("input_t1", 20)
    main2("input", 4000000)
