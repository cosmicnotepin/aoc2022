def  main(filename):
    for line in open(filename, "r"):

    return ""


if __name__ == "__main__":
    res_t = main("input_t1")
    print(f'p2 : {res_t }')
    res_t = main("input")
    print(f'p1 : {res_t }')
