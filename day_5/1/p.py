from collections import defaultdict
import re
import copy

def  main(filename):
    f = open(filename, "r")
    stacks = defaultdict(list)
    for line in f:
        if line[1] == "1":
            break
        for i,c in enumerate(line[1::4]):
            if (c != " "):
                stacks[i+1].insert(0,c)

    f.readline()
    p2_stacks = copy.deepcopy(stacks)
    for line in f:
        co, fr, to = map(int, re.findall("\d+", line))

        p2_stacks[to].extend(p2_stacks[fr][-co:])
        del p2_stacks[fr][-co:]

        for i in range(0,co):
            stacks[to].append(stacks[fr].pop())

    #print("p1: " + "".join([stacks[l][-1]for l in range(1, len(stacks)+1)]))
    print("p1: " + "".join([stacks[l][-1]for l in sorted(stacks.keys())]))
    print("p2: " + "".join([p2_stacks[l][-1] for l in range(1, len(stacks)+1)]))


if __name__ == "__main__":
    main("input_t1")
    main("input")
