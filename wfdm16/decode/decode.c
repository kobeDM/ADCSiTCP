#include <err.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define HEADER_SIZE (4*3)
#define DATA_SIZE (1024*1024)

int usage()
{
    fprintf(stderr, "Usage: prog filename\n");
    return 0;
}

int main(int argc, char *argv[])
{
    FILE *fp;
    unsigned char header_buf[HEADER_SIZE];
    unsigned char data_buf[DATA_SIZE];
    int length_raw;
    int length;
    int trigger_raw;
    int trigger;
    int data_set;
    short data_raw, data;
    int ch;
    int i;
    FILE *outfile[16];
    char filename[1024];
    int c;
    int print_trigger_num = 0;

    while ( (c = getopt(argc, argv, "t")) != -1) {
        switch (c) {
        case 't':
            print_trigger_num = 1;
            break;
        default:
            break;
        }
    }

    argc -= optind;
    argv += optind;
        
    if (argc != 1) {
        usage();
        exit(1);
    }

    if ( (fp = fopen(argv[0], "r")) == NULL) {
        err(1, "fopen");
    }

    for (ch = 0; ch < 16; ch ++) {
        snprintf(filename, sizeof(filename), "ch_%d.txt", ch);
        if ( (outfile[ch] = fopen(filename, "w")) == NULL) {
            err(1, "fopen for %s", filename);
        }
    }

    for ( ; ; ) {
        if (fread(header_buf, 1, HEADER_SIZE, fp) != HEADER_SIZE) {
            //err(1, "fread for header");
            break;
        }

        memcpy(&length_raw, &header_buf[4], 4);
        length = ntohl(length_raw);
        memcpy(&trigger_raw, &header_buf[8], 4);
        trigger = ntohl(trigger_raw);
        //printf("length: %d\n", length);

        if (fread(data_buf, 1, length, fp) != length) {
            //err(1, "fread for data");
            break;
        }
        
        data_set = length / 2 / 16; /* 16 ch */
        //printf("data_set: %d\n", data_set);

        for (i = 0; i < data_set; i++) {
            for (ch = 0; ch < 16; ch ++) {
                int pos = 2*16*i + 2*ch;
                memcpy(&data_raw, &data_buf[pos], 2);
                data = ntohs(data_raw);
                if (print_trigger_num) {
                    fprintf(outfile[ch], "%d ", trigger);
                }
                fprintf(outfile[ch], "%d\n", data);
            }
        }
    }

    for (ch = 0; ch < 16; ch ++) {
        fclose(outfile[ch]);
    }

    return 0;
}
