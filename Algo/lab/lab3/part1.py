#!/usr/bin/env python

def modexp(x, y, N):
    if (y == 0):
        return 1
    z = modexp(x, y >> 1, N)
    if (y % 2 == 0):
       return (z * z) % N
    else:
        return (x * z * z) % N

if __name__ == '__main__':
    print modexp(24, 2015, 1000)
