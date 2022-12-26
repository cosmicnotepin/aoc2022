def  main(filename):
    res = 0
    for line in open(filename, "r").read().splitlines():
        x = len(line)//2
        #dup = ({*line[:x]} & {*line[x:]}).pop() 
        #constructors are not constructors ;)
        dup, = set(line[:x]) & set(line[x:])
        if (ord(dup) < ord('a')):
            res += ord(dup) - ord('A') + 1 + 26
        else:
            res += ord(dup) - ord('a') + 1


    return res


if __name__ == "__main__":
    res_t = main("input_t1")
    print(f'p2 : {res_t }')
    res_t = main("input")
    print(f'p1 : {res_t }')
