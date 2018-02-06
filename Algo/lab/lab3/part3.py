#!/usr/bin/env python
from part2 import primality2

carmichael_numbers = [1105, 1729, 2465, 2821, 6601, 8911, 10585, 15841, 29341, 41041, 46657, 52633, 62745, 63973, 75361, 101101, 115921, 126217, 162401, 172081, 188461, 252601, 278545, 294409, 314821, 334153, 340561, 399001, 410041, 449065, 488881]

for val in carmichael_numbers:
    #times_prime = 0
    #for i in range(0, 1000):
    #    times_prime += primality2(val, 1000)
    #print times_prime
    print primality2(val, 1000)
