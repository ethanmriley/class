#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

struct data {
    int  opcode:16;
    int  block_num:16;
    char data[512];
};

struct acknowledgment {
    int opcode:16;
    int block_num:16;
};

struct error {
    int  opcode:16;
    int  err_code:16;
    char err_msg[40];
};

void main(void)
{
    FILE *fp;
    struct data DATA;
    int bytes_read;

    fp = fopen("test.txt", "r");

    memset(&DATA, 0, sizeof(struct data));
    DATA.opcode = htons(3);
    DATA.block_num = htons(1);
    bytes_read = fread(&DATA.data, 1, 512, fp);

    fwrite(&DATA, (bytes_read + 4), 1, stdout);
}
