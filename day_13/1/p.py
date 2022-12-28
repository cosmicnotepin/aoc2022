from collections import defaultdict
import re
import copy

def cmp(l,r):
    if type(l) == int and type(r) == int:
        return l - r
    if type(l) == int:
        l = [l]
    if type(r) == int:
        r = [r]
    for ll,rr in zip(l,r):
        res = cmp(ll,rr)
        if res == 0:
            continue
        return res
    return len(l) - len(r)

        
def  main(filename):
    f = open(filename, "r")
    pairs = list(map(str.splitlines, f.read().split('\n\n')))
    f.close()
    res = 0
    for i, (l,r) in enumerate(pairs):
        if cmp(eval(l),eval(r)) <= 0:
            res += i+1

    f = open(filename, "r")
    pairs = [eval(x) for x in f.read().split('\n') if x != ''] + [[[2]]] + [[[6]]]

    import functools
    pairs.sort(key=functools.cmp_to_key(cmp))
    print("p1: " + str(res))
    print("p2: " + str((pairs.index([[2]]) + 1) * (pairs.index([[6]])+1)))


if __name__ == "__main__":
    main("input_t1")
    main("input")
