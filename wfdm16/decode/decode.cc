#include "inc/shinclude.h"
#include <err.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

const int    NUM_CH       = 16;
const size_t PACKET_WIDTH = 4;
const int    MAX_SAMPLING = 4096;

#define HEADER_SIZE (4*3)
#define DATA_SIZE (1024*1024)

int usage()
{
    fprintf(stderr, "Usage: prog filename\n");
    return 0;
}

int main(int argc, char *argv[])
{
    unsigned char header_buf[HEADER_SIZE];
    unsigned char data_buf[DATA_SIZE];
    int length_raw;
    int length;
    int trigger_raw;
    int data_set;
    short data_raw, data;
    int i;
    std::vector< std::ofstream* > outfileArr;
    outfileArr.reserve( NUM_CH );
    char filename[1024];

    int option;
    int print_trigger_num = 0;
    while( (option = getopt(argc, argv, "t")) != -1 ){
        switch (option) {
        case 't':
            print_trigger_num = 1;
            break;
        default:
            break;
        }
    }

    if (argc != 2) {
        usage();
        exit(1);
    }

    String input = argv[1];
    std::ifstream ifs( input, std::ios::in | std::ios::binary );
    if( ifs.is_open( ) == false ) {
        return 0;
    }

    for( int ch = 0; ch < NUM_CH; ch ++) {
        std::ofstream* pOfs = new std::ofstream( Form( "ch_%d.txt", ch ) );
        if( pOfs->is_open( ) == false ) abort();
        outfileArr.push_back( pOfs );
    }

    std::cout.fill('0');
    char  buf[PACKET_WIDTH] = {}; // 1 byte
    char  adc[MAX_SAMPLING*2] = {}; // 2 byte
    int  trigger = 0;
    int  hID      = 0, hTRG_POS = 0;               // word 1
    int  hSMP_FRG = 0, hCH_TOP  = 0, hCH_STP  = 0; // word 2
    int  hCH_NUM  = 0, hDT_LEN  = 0, hREC_LEN = 0; // word 3
    while( !ifs.eof( ) ) {
        ifs.read( buf, PACKET_WIDTH );

        // read header
        if( (int)buf[0] == 0xaa && (int)buf[1] == 0x55 ) {
            hID      = ((int)buf[0] << 8) + (int)buf[1];
            hTRG_POS = ((int)buf[2] << 8) + (int)buf[3];

            std::cout << "HEAD_ID: " << std::hex << std::setw(4) << hID      << std::endl;
            std::cout << "TRG_POS: " << std::hex << std::setw(4) << hTRG_POS << std::endl;

            ifs.read( buf, PACKET_WIDTH );
            hSMP_FRG = ((int)buf[0] << 8) + (int)buf[1];
            hCH_TOP  = (int)buf[2];
            hCH_STP  = (int)buf[3];
            
            std::cout << "SMP_FRG: " << std::dec << hSMP_FRG << std::endl;
            std::cout << "CH_TOP : " << std::dec << hCH_TOP  << std::endl;
            std::cout << "CH_STP : " << std::dec << hCH_STP  << std::endl;

            ifs.read( buf, PACKET_WIDTH );
            hCH_NUM  = (int)buf[0];
            hDT_LEN  = (int)buf[1];
            hREC_LEN = ((int)buf[2] << 8) + (int)buf[3];
            
            std::cout << "CH_NUM : " << std::dec << hCH_NUM  << std::endl;
            std::cout << "DT_LEN : " << std::dec << hDT_LEN  << std::endl;
            std::cout << "REC_LEN: " << std::dec << hREC_LEN << std::endl;

            ifs.read( buf, PACKET_WIDTH );
            std::cout << "TRIG_TIM1 : " << std::hex << (int)buf[0] << (int)buf[1] << (int)buf[2] << (int)buf[3]  << std::endl;
            ifs.read( buf, PACKET_WIDTH );
            std::cout << "TRIG_TIM2 : " << std::hex << (int)buf[0] << (int)buf[1] << (int)buf[2] << (int)buf[3]  << std::endl;

            std::cout << std::dec;
            for( int ch = 0; ch < hCH_NUM; ++ch ) {
                ifs.read( adc, (int)(hREC_LEN * sizeof(short)) );
                std::ofstream* pOfs = outfileArr.at( ch );
                if( pOfs == nullptr ) continue;
                for( int clk = 0; clk < hREC_LEN; ++clk ) {
                    // DEBUG( ((int)adc[2*clk] << 4 ) + ((int)adc[2*clk+1] >> 4 ));
                    *pOfs << trigger << "\t" << ((int)adc[2*clk] << 4 ) + ((int)adc[2*clk+1] >> 4 ) << std::endl;
                }
            }
            ++trigger;
        }
        
        /* if (fread(header_buf, 1, HEADER_SIZE, fp) != HEADER_SIZE) { */
        // if (fread(header_buf, 4, 1, fp) != 1) {
        //     printf("break\n");
        //     /* err(1, "fread for header"); */
        //     break;
        // }
        
        /* memcpy(&length_raw, &header_buf[4], 4); */
        /* length = ntohl(length_raw); */
        /* memcpy(&trigger_raw, &header_buf[8], 4); */
        /* trigger = ntohl(trigger_raw); */
        /* //printf("length: %d\n", length); */

        /* if (fread(data_buf, 1, length, fp) != length) { */
        /*     //err(1, "fread for data"); */
        /*     break; */
        /* } */
        
        /* data_set = length / 2 / 16; /\* 16 ch *\/ */
        /* //printf("data_set: %d\n", data_set); */

        /* for (i = 0; i < data_set; i++) { */
        /*     for (ch = 0; ch < 16; ch ++) { */
        /*         int pos = 2*16*i + 2*ch; */
        /*         memcpy(&data_raw, &data_buf[pos], 2); */
        /*         data = ntohs(data_raw); */
        /*         if (print_trigger_num) { */
        /*             fprintf(outfile[ch], "%d ", trigger); */
        /*         } */
        /*         fprintf(outfile[ch], "%d\n", data); */
        /*     } */
        /* } */
    }

    for( auto pOfs : outfileArr ) {
        DEBUG("close");
        if( pOfs == nullptr ) continue;
        pOfs->close( );
        delete pOfs;
    }

    return 0;
}
