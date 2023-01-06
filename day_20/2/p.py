from collections import defaultdict
import re
import copy

def  main(filename):
    f = open(filename, "r")
    dk = 811589153
    orig = []
    l = []
    i = 0
    z_ii = 0
    for line in f:
        val = dk * int(line)
        orig.append((i,val))
        l.append((i,val))
        if(val == 0):
            z_ii = i
        i+=1


    for _ in range(10):
        for o in orig:
            old_i = l.index(o)
            val = l.pop(old_i)
            l.insert((old_i + val[1]) % len(l), val)

    z_i = l.index((z_ii, 0))
    print("p1: " + str(l[(z_i+1000)%len(l)][1] \
        + l[(z_i+2000)%len(l)][1] \
        + l[(z_i+3000)%len(l)][1]))
    print("p2: " + "")


if __name__ == "__main__":
    main("input_t1")
    main("input")
