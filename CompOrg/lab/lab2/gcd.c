#include <stdio.h>

int gcd(unsigned int a, unsigned int b ) {
    if(b == 0) {
        return a;
    }
    else {
        return gcd(b, a % b);
    }
}

void main(void) {
    unsigned int a, b, g;
    
    printf("a: ");
    scanf("%u", &a);
    printf("b: ");
    scanf("%u", &b);
    
    g = gcd(a, b);
    
    printf("%u\n", g);
}
