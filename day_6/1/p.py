from collections import defaultdict
import copy

def  main(filename):
    s = open(filename, "r").read()
    ml=4
    for i in range(ml,len(s)):
        if(len(set(s[i-ml:i])) == ml):
            print("p1: "+str(i))
            break
    ml=14
    for i in range(ml-1,len(s)):
        if(len(set(s[i-ml:i])) == ml):
            print("p2: "+str(i))
            break
            

if __name__ == "__main__":
    main("input_t1")
    main("input")
