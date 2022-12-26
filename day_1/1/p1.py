def  main():
    elves = [0]
    for line in open("input", "r"):
        if line == "\n":
            elves.append(0)
            continue
        elves[-1] += int(line.strip())

    print("p1: " + str(max(elves)))
    print("p2: " + str(sum(sorted(elves)[-3:])))


if __name__ == "__main__":
    main()
