#!/usr/bin/env python

def sort(A):
    max_val, min_val = A[0], A[0]

    for val in A:
        if val > max_val:
            max_val = val
        elif val < min_val:
            min_val = val
    
    sorted_A = []
    counts = []

    for i in range(min_val, max_val + 1):
        counts.append([i, 0])

    for val in A:
        counts[val - min_val][1] += 1

    for count in counts:
        for j in range(0, count[1]):
            sorted_A.append(count[0])
    
    return sorted_A

if __name__ == '__main__':
    A = [5, 3, 4, 6, 1, 7 ,3, 4, 7]
    print sort(A)
