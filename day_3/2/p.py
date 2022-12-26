def  main(filename):
    res = 0
    lines = open(filename, "r").read().splitlines()
    while (len(lines) > 0):
        dup = (set(lines.pop(0)) & set(lines.pop(0)) & set(lines.pop(0))).pop()
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
