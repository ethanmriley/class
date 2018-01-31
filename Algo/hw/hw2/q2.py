#!/usr/bin/env python

def q2(n):
    if(n == 1):
        return 1
    return q2(n-1) * n

if __name__ == '__main__':
    import sys

    for i in range(1, 30):
        result = q2(i)
        #print 'n: {}\nresult: {}\nbits: {}'.format(i, result, result.bit_length())
        print str(i / result.bit_length())
	
