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
    char A[17] = "0000000000000000\0";
    char B[17] = "0000000000000000\0";
    int subtract = -999;

    char temp[16] = {0};
    
    printf("Enter A (hex):\n");
    fgets(temp, sizeof(temp), stdin);
    memcpy((A + (sizeof(A) - strlen(temp))), temp, strlen(temp));

    memset(temp, 0, sizeof(temp));

    printf("Enter B (hex):\n");
    fgets(temp, sizeof(temp), stdin);
    memcpy((B + (sizeof(B) - strlen(temp))), temp, strlen(temp)); 

    memset(temp, 0, sizeof(temp));

    printf("Add (0) or subtract (1):\n");
    fgets(temp, sizeof(temp), stdin);

    if(strcmp("1\n", temp) == 0)
        subtract = 1;
    else if(strcmp("0\n", temp) == 0)
        subtract = 0;

    return 0;
}
