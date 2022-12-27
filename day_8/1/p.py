from collections import defaultdict
import re
import copy

def  main(filename):
    #just keep and compare charaters
    grid = open(filename, "r").read().splitlines()
    nt = 0
    bsc = 0
    for y in range(len(grid)):
        for x in range(len(grid[0])):
            t = grid[y][x]
            if all(grid[r][x] < t for r in range(y)) or \
               all(grid[r][x] < t for r in range(y+1, len(grid))) or \
               all(grid[y][c] < t for c in range(x)) or \
               all(grid[y][c] < t for c in range(x+1, len(grid[0]))):
                   nt+=1
            dirs = [[grid[r][x] >= t for r in range(y-1,-1,-1)],\
                    [grid[r][x] >= t for r in range(y+1, len(grid))],\
                    [grid[y][c] >= t for c in range(x-1,-1,-1)],\
                    [grid[y][c] >= t for c in range(x+1, len(grid[0]))]]

            sc = 1;
            for d in dirs:
                if True in d:
                    sc *= d.index(True) + 1
                else:
                    if len(d) > 0:
                        sc *= len(d)
            bsc = max(bsc, sc)

    print("p1: " + str(nt))
    print("p2: " + str(bsc))


if __name__ == "__main__":
    main("input_t1")
    main("input")
