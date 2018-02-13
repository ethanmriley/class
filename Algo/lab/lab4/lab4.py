#!/usr/bin/env python
import random

def gen_array(n):
    arr = []
    for i in range(0, n):
        arr.append(random.randint(0, n - 1))
    return arr

def select(arr, k):
    arr_l, arr_v, arr_r = [], [], []

    v = arr[random.randint(0, len(arr) - 1)]
    
    #populate our sub-arrays
    for val in arr:
        if val < v:
            arr_l.append(val)
        elif val == v:
            arr_v.append(val)
        elif val > v:
            arr_r.append(val)

    len_l, len_v, len_r = len(arr_l), len(arr_v), len(arr_r)

    if k <= len_l:
        return select(arr_l, k)
    elif len_l < k <= len_l + len_v:
        return v
    elif k > len_l + len_v:
        return select(arr_r, k - len_l - len_v)
    

if __name__ == '__main__':
    import sys
    if(len(sys.argv) >= 3):
        n, k = int(sys.argv[1]), int(sys.argv[2])
    else:
        n, k = 10, 3
    arr = gen_array(n)
    print "select {}".format(select(arr, k))
    print "array {}".format(arr)
    print "sorted array {}".format(sorted(arr))
