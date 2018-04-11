#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <math.h>

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
uint32_t get_index(uint32_t address, uint32_t idx_start, uint32_t idx_end)
{
    // assert() calls are great to ensure your logic is valid!
    assert(idx_start < idx_end);

    return 0;
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
    assert(bob + 1 == idx_start);
    assert(idx_start < idx_end);
    
    return 0;
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
    printf("get_cache_line_size(3, 16, 2) returned %d\n\n",
           get_cache_line_size(3, 16, 2));
    
    printf("get_cache_line_size(4, 15, 3) returned %d\n\n",
           get_cache_line_size(4, 15, 3));
    
    
    return 0;
}
