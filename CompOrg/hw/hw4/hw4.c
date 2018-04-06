#include <stdio.h>
#include <string.h>

char* hexToBin(char* hex) {
    const char* quads[16]= {"0000", "0001", "0010", "0011", "0100", "0101", "0110", "0111", "1000", "1001", "1010", "1011", "1100", "1101", "1110", "1111" };
    char* result = "";

    for(unsigned int i = 0; i < 64; i++) {
        unsigned char c = hex[i];
        if (c >= '0' && c <= '9') strcat(result, quads[     c - '0']);
        if (c >= 'A' && c <= 'F') strcat(result, quads[10 + c - 'A']);
        if (c >= 'a' && c <= 'f') strcat(result, quads[10 + c - 'a']);
    }

    return result;
}

int main() {
    return 0;
}
