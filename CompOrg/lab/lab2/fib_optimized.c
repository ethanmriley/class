#include <stdio.h>

long fib(int n){
    if(n == 0) { return 0; }
    if(n == 1) { return 1; }

    long fib_values[100] = { 0 };
    fib_values[1] = 1;

    for(unsigned int i = 2; i <= n; i++) {
        fib_values[i] = fib_values[i - 1] + fib_values[i - 2];
    }

    return fib_values[n];
}


int main(void) {
    int  n = 0;
    long f = 0;
    
    printf("n: ");
    scanf("%d", &n);
    
    f = fib(n);
    
    printf("%ld\n", f);
}
