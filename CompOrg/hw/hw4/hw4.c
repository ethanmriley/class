#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

void hexToBin(char* hex, char* result) {
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

    return;
}

void decToHex(int dec, char* result) {
    sprintf(result, "%X", dec);
    return;
}

int add_two_bits(int d0, int d1, int carry_in, int *carry_out) {
    *carry_out = ((d0 ^ d1) & (carry_in)) || (d0 & d1);
    return (d0 ^ d1) ^ carry_in;
}

void add(char* A_bin, char* B_bin, char* S_bin) {
    int g[64] = {0};
    int p[64] = {0};
    int gg[16] = {0};
    int gp[16] = {0};
    int sg[4] = {0};
    int sp[4] = {0};
    int sc[4] = {0};
    int gc[16] = {0};
    int c[64] = {0};
    int sum[64] = {0};
    int A_int[64] = {0};
    int B_int[64] = {0};
    int A;
    int B;
    int carry_in;

    for(unsigned int i = 0; i < 64; i++) {
        A = (A_bin[i] == '1');
        B = (B_bin[i] == '1');
        
        A_int[i] = A;
        B_int[i] = B;
       
        if(A && B) {
            g[i] = 1;
        }
        else if(A ^ B) {
            p[i] = 1;
        }
    }

    for(unsigned int j = 0; j < 16; j++) {
        gg[j] = (g[(j*4)+3]==1 || (g[(j*4)+2]==1 && p[(j*4)+3]==1) || (g[(j*4)+1]==1 && p[(j*4)+2]==1 && p[(j*4)+3]==1) || (g[(j*4)]==1 && p[(j*4)+3]==1 && p[(j*4)+2]==1 && p[(j*4)+1]==1));
        gp[j] = (p[j*4]==1 && p[(j*4)+1]==1 && p[(j*4)+2]==1 && p[(j*4)+3]==1);
    }

    for(unsigned int k = 0; k < 4; k++) {
        sg[k] = gg[k*4+3] || (gg[k*4+2] && gp[k*4+3]) || (gg[k*4+1] && gp[k*4+2] && gp[k*4+3]) || (gg[k*4] && gp[k*4+3] && gp[k*4+2] && gp[k*4+1]);
        sp[k] = gp[k*4] && gp[k*4+1] && gp[k*4+2] && gp[k*4+3];
    }

    for(unsigned int l = 0; l < 4; l++) {

        if(l == 0)
            carry_in = 0;
        else
            carry_in = sc[l-1];

        sc[l] = sg[l] || (sp[l] && carry_in);
    }

    for(unsigned int m = 0; m < 16; m++) {
        if(m % 4 == 0)
            carry_in = sc[m/4];

        gc[m] = gg[m] || (gp[m] && carry_in);
    }

    for(unsigned int n = 0; n < 64; n++) {
        if(n % 16 == 0)
            carry_in = sc[n/16];

        c[n] = g[n] || (p[n] && carry_in);
    }

    for(unsigned int o = 0; o < 64; o++) {
        if(o == 0)
            carry_in = 0;
        else
            carry_in = c[o];

        sum[o] = A_int[o] ^ B_int[o] ^ carry_in;
    }

    for(unsigned int p = 0; p < 64; p++) {
        if(sum[p]==1)
            S_bin[p] = '1';
        else
            S_bin[p] = '0';
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
    printf("\n");

    S_long = strtoul(S_bin, 0, 2);

    memset(temp, 0, sizeof(temp));
    decToHex(S_long, temp);

    for(unsigned int i = 0; i < strlen(temp); i++) {
        temp[i] = tolower(temp[i]);
    }

    memcpy((S + (sizeof(S) - strlen(temp))), temp, strlen(temp));

    printf("S is %s or %lu\n", S, S_long);

    return 0;
}
