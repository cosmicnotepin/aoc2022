from collections import defaultdict
import re
import copy

def  main(filename):
    monkeys = []
    for lines in open(filename, "r").read().split('\n\n'):
        lines = lines.splitlines()
        m = []
        m.append(list(map(int, lines[1].split(': ')[1].split(', '))))
        m.append(eval('lambda old: ' + lines[2].split('=')[1]))
        m.append(int(lines[3].split('by ')[1]))
        m.append(int(lines[4].split('monkey ')[1]))
        m.append(int(lines[5].split('monkey ')[1]))
        monkeys.append(m)

    nr_insps = [0]*len(monkeys)
    for _ in range(20):
        for i, m in enumerate(monkeys):
            for _ in range(len(m[0])):
                nr_insps[i]+=1
                wl = m[1](m[0].pop(0)) // 3
                if wl % m[2]: 
                    monkeys[m[4]][0].append(wl)
                else:
                    monkeys[m[3]][0].append(wl)

    nr_insps.sort(reverse=True)
    print("p1: " + str(nr_insps[0] * nr_insps[1]))
    print("p2: " + "")


if __name__ == "__main__":
    main("input_t1")
    main("input")
