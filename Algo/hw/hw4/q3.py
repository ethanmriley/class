#!/usr/bin/env python

def find_min(A):
    #base case
    if(len(A) == 2):
        if A[0] < A[1]:
            return A[0]
        else if A[1] <= A[0]:
            return A[1]

    for i in range(len(A)):

