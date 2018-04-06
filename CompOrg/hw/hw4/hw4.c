#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char* hexToBin(char* hex, char* result) {
    char* quads[16]= {"0000", "0001", "0010", "0011", "0100", "0101", "0110", "0111", "1000", "1001", "1010", "1011", "1100", "1101", "1110", "1111" };

    for(unsigned int i = 0; i < 16; i++) {
        unsigned char c = hex[i];
        if (c >= '0' && c <= '9') {
            strcat(result, quads[c - '0']);
        }
        if (c >= 'A' && c <= 'F') {
            strcat(result, quads[10 + c - 'A']);
        }
        if (c >= 'a' && c <= 'f') {
            strcat(result, quads[10 + c - 'a']);
        }
    }

    return result;
}

int add_two_bits(int d0, int d1, int carry_in, int *carry_out) {
    *carry_out = ((d0 ^ d1) & (carry_in)) || (d0 & d1);
    return (d0 ^ d1) ^ carry_in;
}

void add(char* A_bin, char* B_bin, char* S_bin) {
    char g[64] = {'\30'};
    char p[64] = {'\30'};
    char gg[16] = {'\30'};
    char gp[16] = {'\30'};
    char sg[4] = {'\30'};
    char sp[4] = {'\30'};
    int A;
    int B;

    for(unsigned int i = 0; i < 64; i++) {
        A = A_bin[i] == '1';
        B = B_bin[i] == '1';
        
        if(A && B)
            g[i] = '1';
        else if(A ^ B)
            p[i] = '1';
    }

    return;
}

void subtract(char* A_bin, char* B_bin, char* S_bin) {
    return;
}

void invert(char* B_bin, long int B_long) {
    printf("Inverting %ld\n", B_long);

    for(unsigned int i = 0; i < strlen(B_bin); i++) {
        if(B_bin[i] == '1')
            B_bin[i] = '0';
        else if (B_bin[i] == '0')
            B_bin[i] = '1';
    }

    printf("B (bin) : %s\n", B_bin);
    return;
}

int main() {
    char A[17] = "0000000000000000\0";
    char B[17] = "0000000000000000\0";
    char S[17] = "0000000000000000\0";
    unsigned long int A_long = 0;
    unsigned long int B_long = 0;
    unsigned long int S_long = 0;
    char A_bin[65] = {0};
    char B_bin[65] = {0};
    char S_bin[65] = {0};

    char temp[256] = {0};
    
    printf("Enter A (hex):\n");
    fgets(temp, sizeof(temp), stdin);
    memcpy((A + (sizeof(A) - strlen(temp))), temp, strlen(temp));

    memset(temp, 0, sizeof(temp));

    printf("Enter B (hex):\n");
    fgets(temp, sizeof(temp), stdin);
    memcpy((B + (sizeof(B) - strlen(temp))), temp, strlen(temp)); 

    A[16] = '\0';
    B[16] = '\0';

    memset(temp, 0, sizeof(temp));

    memcpy(temp, "0x", 2);
    memcpy(temp+2, A, sizeof(A));
    A_long = strtoul(temp, 0, 16);
    memcpy(temp, "0x", 2);
    memcpy(temp+2, B, sizeof(B));
    B_long = strtoul(temp, 0, 16);

    hexToBin(A, A_bin);
    hexToBin(B, B_bin);

    printf("Add (0) or subtract (1):\n");
    fgets(temp, sizeof(temp), stdin);

    printf("\n");
    printf("A is %s or %lu\n", A, A_long);
    printf("B is %s or %lu\n", B, B_long);

    if(strcmp("0\n", temp) == 0)
        add(A_bin, B_bin, S_bin);
    else if(strcmp("1\n", temp) == 0)
        invert(B_bin, B_long);
        subtract(A_bin, B_bin, S_bin);

    printf("\n");
    printf("Calculate sum, S:\n\n");
    printf("A (bin) : %s\n", A_bin);
    printf("B (bin) : %s\n", B_bin);
    printf("S (bin) : %s\n", S_bin);

    return 0;
}
