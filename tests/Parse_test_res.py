#!/usr/bin/python

import os, sys
import config

def main():
    fname = sys.argv[1]
    no_res = []
    bad_assert = []
    num_of_tests = 0

    with open(fname, 'r') as f:
        l = f.readline()
        while l:
            num_of_tests += 1
            if l[0] == 'F':
                id = l.split(' -- ')[1].rstrip()[:-1]
                n = f.readline()
                if 'No response from server' in n:
                    no_res.append(id)
                else:
                    bad_assert.append(id)
            l = f.readline()    

    print("Total tests attempted: {}".format(num_of_tests))
    print("No response from server: {}".format(no_res))
    print("Assert failure: {}".format(bad_assert))
    known_fail = config.failing_tests
    unknown_fail = [x for x in bad_assert if x not in known_fail]
    print("unknown failure: {}".format(unknown_fail))
    if unknown_fail or len(no_res) > num_of_tests/10:
        print("Test failed")
        exit(1)

if __name__ == "__main__":
    main()



