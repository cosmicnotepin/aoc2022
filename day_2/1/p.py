def  main(filename):
    score = 0
    for line in open(filename, "r").read().splitlines():
        [op, me] = line.split(" ");
        op = ord(op) - ord('A')
        me = ord(me) - ord('X')
        score += ((me-op+1) % 3) * 3 + me + 1

    return score


if __name__ == "__main__":
    res_t = main("input_t1")
    print(f't1: {res_t }')
    res_t = main("input")
    print(f'p1: {res_t }')
