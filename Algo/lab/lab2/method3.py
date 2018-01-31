#!/usr/bin/env python

def method3(x, y):
    xdec = int(x, 2)
    ydec = int(y, 2)
       
    n = max(len(x), len(y))
    if n == 1:
        return xdec*ydec

    lshift = (n - (n >> 1)) 
    xl = x[:lshift] 
    xr = x[lshift::]
    yl = y[:lshift]
    yr = y[lshift::]

    #print 'xl: {}  xr: {}  yl: {}  yr: {}'.format(xl, xr, yl, yr)

    p1 = method3(xl, yl)
    p2 = method3(xr, yr)
    xsum = bin(int(xr, 2) + int(xl, 2))[2::]
    ysum = bin(int(yr, 2) + int(yl, 2))[2::]
    p3 = method3(xsum, ysum)

    #print 'p1: {}  p2: {}  p3: {} n: {}'.format(p1, p2, p3, n)

    return ((p1 << ( (n >> 1) << 1)) + ((p3 - p1 - p2) << (n >> 1)) + p2)


if __name__ == '__main__':
    binv = bin(5)[2::]
    print method3(binv, binv)
