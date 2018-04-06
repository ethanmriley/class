#include <stdio.h>
#include <string.h>

char* hexToBin(char* hex, char* result) {
    char* quads[16]= {"0000", "0001", "0010", "0011", "0100", "0101", "0110", "0111", "1000", "1001", "1010", "1011", "1100", "1101", "1110", "1111" };

    for(unsigned int i = 0; i < 64; i++) {
        unsigned char c = hex[i];
        if (c >= '0' && c <= '9') {
            printf("%s\n", quads[c - '0']);
            strcat(result, quads[c - '0']);
        }
        if (c >= 'A' && c <= 'F') {
            printf("%s\n", quads[10 + c - 'A']);
            strcat(result, quads[10 + c - 'A']);
        }
        if (c >= 'a' && c <= 'f') {
            printf("%s\n", quads[10 + c - 'a']);
            strcat(result, quads[10 + c - 'a']);
        }
    }

    return result;
}

int main() {
    char* deadbeef_hex = "deadbeef";
    char  deadbeef_bin[65];
    hexToBin(deadbeef_hex, deadbeef_bin);
    printf("%s\n", deadbeef_bin);
    return 0;
}
