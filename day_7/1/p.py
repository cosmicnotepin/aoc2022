def  main(filename):
    fs = {}
    paths = []
    for line in open(filename, "r"):
        line = line.strip()
        if line[0] == '$':
            if line[2] == 'c':
                tar = line[5:]
                if tar == '/':
                    paths = ['/']
                    fs[paths[-1]] = 0
                    continue
                if tar == '..':
                    paths.pop()
                    continue
                paths.append(paths[-1] + tar + '/')
                if not paths[-1] in fs:
                    fs[paths[-1]] = 0
        else:
           sz, nm = line.split() 
           if (sz[0] == 'd'):
               continue
           for p in paths:
               fs[p] += int(sz)

    print("p1: " + str(sum([x for x in fs.values() if x<=100000])))
    max_used = 40000000
    to_del = fs['/'] - 40000000

    best = fs['/'] 
    for p in fs.values():
        if p<best and p>=to_del:
            best = p

    print("p2: " + str(best))


if __name__ == "__main__":
    main("input_t1")
    main("input")
