#!/usr/bin/env python

def method1(x, y):
    xbin = bin(x)[2::]
    ybin = bin(y)[2::]
    print 'xbin: {}  ybin: {}'.format(xbin, ybin)

    result = 0
    for i in range(len(ybin)):
        result += (x * int(ybin[::-1][i])) << i
    return result

if __name__ == '__main__':
    print method1(13, 11)
