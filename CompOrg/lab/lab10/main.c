#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>

/*
 Given the index values, determine the number of slots in your cache.
 */
uint32_t slot_count(uint32_t idx_start, uint32_t idx_end)
{
    // assert() calls are great to ensure your logic is valid!
    assert(idx_start < idx_end);

    return pow(2, (idx_end-idx_start+1));
}

/*
 Given an address and range of bits in the index (start-end, inclusive)
 return the index.  Example:
 
 Given the address below, start of 4, and end of 11
 
    2    1    3    5    C    6    9    0
 0010 0001 0011 0101 1100 0110 1001 0000
                          ^       ^
                          e       s
 
 this function should return 0110 1001 or 0x69 or 105 in decimal.
 */

int get_bit(uint32_t num, int index) {
    return (num & (1 << index)) >> index;
}

uint32_t get_index(uint32_t address, uint32_t idx_start, uint32_t idx_end)
{
    // assert() calls are great to ensure your logic is valid!
    assert(idx_start < idx_end);

    int bit;
    char index[20] = {'\30'};
    int i = idx_end;
    int j = 0;

    for(; i >= idx_start; i--) {
        bit = get_bit(address, i);
        if(bit == 1) {
            index[j] = '1';
        } else {
            index[j] = '0';
        }
        j++;
    }

    return strtoul(index, NULL, 2);
}

/*
 Utilizing slide 33 as a reference, calculate the total BITS in the cache
 LINE including data, tag, and valid bits.  Assume 32-bit addressing.
 The bob argument should be the leftmost bit number of the BOB,
 e.g., 3 means bits 0-3 (inclusive) are used for the BOB.
 */
uint32_t get_cache_line_size(uint32_t idx_start, uint32_t idx_end, uint32_t bob)
{
    // assert() calls are great to ensure your logic is valid!
    int data;
    data = pow(2, bob+1);
    data *= 8;
    assert(bob + 1 == idx_start);
    assert(idx_start < idx_end);
    
    return (32 - idx_end + data + 1);
}

int main()
{
    // This should return 256
    printf("slot_count(4, 11) returned %d\n\n",
           slot_count(4, 11));
    
    printf("slot_count(5, 13) returned %d\n\n",
           slot_count(5, 13));
    
    // This should return 105
    printf("get_index(0x2135C690, 4, 11) returned %d\n\n",
           get_index(0x2135C690, 4, 11));
    
    printf("get_index(0x2135C690, 5, 17) returned %d\n\n",
           get_index(0x2135C690, 5, 17));
    
    // This should return 49
    printf("get_cache_line_size(2, 16, 1) returned %d\n\n",
           get_cache_line_size(2, 16, 1));
    
    printf("get_cache_line_size(4, 15, 3) returned %d\n\n",
           get_cache_line_size(4, 15, 3));
    
    
    return 0;
}
