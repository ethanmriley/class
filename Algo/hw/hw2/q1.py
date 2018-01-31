#!/usr/bin/env python

def q1(n):
	#compute 2**(2**n)
	exp = 2 << ( n  - 1) 
	res = 2 << (exp - 1)
	return res

if __name__ == '__main__':
	import sys
	print q1(int(sys.argv[1]))
