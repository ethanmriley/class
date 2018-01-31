#!/usr/bin/env python

def method2(x, y):
    if y == 0:
        return 0
    z = method2(x, y >> 1)
    if (y % 2) == 0:
        return z << 1
    else:
        return x + (z << 1)


if __name__ == '__main__':
    print method2(11, 13)
