def  main(filename):
    score_p1 = 0
    score_p2 = 0
    for line in open(filename, "r").read().splitlines():
        [op, me] = line.split(" ");
        op = ord(op) - ord('A')
        me = ord(me) - ord('X')
        score_p1 += ((me-op+1) % 3) * 3 + me + 1
        score_p2 += (op + me - 1) % 3 + 1 + me * 3

    return score_p1, score_p2


if __name__ == "__main__":
    res_t1, res_t2= main("input_t1")
    print(f't1: {res_t1}, t2: {res_t2}')
    res_t1, res_t2 = main("input")
    print(f'p1: {res_t1}, p2: {res_t2}')
