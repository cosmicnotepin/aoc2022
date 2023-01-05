import re
import operator

def  main(filename):
    pat = re.compile('\d+');
    f = open(filename, "r")
    solids = {tuple(map(int, pat.findall(line))) for line in f}
    #solids = {tuple(int(x) for x in pat.findall(line)) for line in f}

    res = 0
    nos = [[0,0,1], [0,0,-1], [0,1,0], [0,-1,0], [1,0,0], [-1,0,0]]
    for s in solids:
        res += 6 - len({tuple(map(sum, zip(s, n))) for n in nos} & solids)

    print("p1: " + str(res))

    minv = tuple(min(x)-1 for x in zip(*solids))
    maxv = tuple(max(x)+1 for x in zip(*solids))

    res = 0
    visited = set() | solids
    todo = [tuple(minv)]
    while len(todo) > 0:
        cur = todo.pop(0)
        ns = {tuple(map(sum, zip(cur, n))) for n in nos}
        ns = {e for e in ns if all(mi<=ec for mi,ec in zip(minv, e)) and all(ma>=ec for ma,ec in  zip(maxv, e))} 
        res += len(ns & solids) 
        todo.extend(ns - solids - visited)
        visited |= (ns - solids)

    print("p2: " + str(res))


if __name__ == "__main__":
    main("input_t1")
    main("input")
