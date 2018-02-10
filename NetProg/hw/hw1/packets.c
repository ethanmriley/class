#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

struct read_write_request {
    int  opcode:16;
    char filename[256];
    char mode[6];
};

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
    struct read_write_request RRQ;

    memset(&RRQ, 0, 264);

    RRQ.opcode = htons(1);
    strcpy(RRQ.filename, "test.txt");
    strcpy(RRQ.mode, "octet");
    
    fwrite(&RRQ, 264, 1, stdout);
}
