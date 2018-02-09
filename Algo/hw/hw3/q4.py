#!/usr/bin/env python

def exp(x, y):
    if y == 0:
        return 1
    z = exp(x, y >> 1)
    if y % 2 == 0:
        return z*z
    else:
        return x*z*z

if __name__ == '__main__':
    import sys
    x, y = int(sys.argv[1]), int(sys.argv[2])
    print exp(x, y)
