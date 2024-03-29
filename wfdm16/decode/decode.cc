#include "inc/shinclude.h"

const int    NUM_CH       = 16;
const size_t PACKET_WIDTH = 4;
const int    MAX_SAMPLING = 4096;

int usage()
{
    ShUtil::Cinfo( "Usage: ./decode [input filename] [output filename]" );
    return 0;
}

int main(int argc, char *argv[])
{
    std::vector< std::ofstream* > outfileArr;
    outfileArr.reserve( NUM_CH );
    char filename[1024];

    if (argc != 3) {
        usage();
        exit(1);
    }

    String input = argv[1];
    std::ifstream ifs( input, std::ios::in | std::ios::binary );
    if( ifs.is_open( ) == false ) {
        return 0;
    }

    String output = argv[2];
    TFile file( output.c_str( ), "RECREATE" );
    TTree* pTree = new TTree( "tree", "tree" );

    std::cout.fill('0');
    char  buf[PACKET_WIDTH]   = {}; // 1 byte
    char  adc[MAX_SAMPLING*2] = {}; // 2 byte
    int   trigger  = 0;
    int   hID      = 0, hTRG_POS = 0;               // word 1
    int   hSMP_FRG = 0, hCH_TOP  = 0, hCH_STP  = 0; // word 2
    int   hCH_NUM  = 0, hDT_LEN  = 0, hREC_LEN = 0; // word 3
    
    int   fadcVar[NUM_CH][MAX_SAMPLING] = {};
    pTree->Branch( "fadc0", fadcVar[0], "fadc0[2048]/I" );
    pTree->Branch( "fadc1", fadcVar[1], "fadc1[2048]/I" );
    pTree->Branch( "fadc2", fadcVar[2], "fadc2[2048]/I" );
    pTree->Branch( "fadc3", fadcVar[3], "fadc3[2048]/I" );
    pTree->Branch( "fadc4", fadcVar[4], "fadc4[2048]/I" );
    pTree->Branch( "fadc5", fadcVar[5], "fadc5[2048]/I" );
    pTree->Branch( "fadc6", fadcVar[6], "fadc6[2048]/I" );
    pTree->Branch( "fadc7", fadcVar[7], "fadc7[2048]/I" );

    std::cout << "Decode start." << std::endl;
    while( !ifs.eof( ) ) {
        ifs.read( buf, PACKET_WIDTH );

        // read header
        if( (int)buf[0] == 0xaa && (int)buf[1] == 0x55 ) {
            hID      = ((int)buf[0] << 8) + (int)buf[1];
            hTRG_POS = ((int)buf[2] << 8) + (int)buf[3];

            // std::cout << "HEAD_ID: " << std::hex << std::setw(4) << hID      << std::endl;
            // std::cout << "TRG_POS: " << std::hex << std::setw(4) << hTRG_POS << std::endl;

            ifs.read( buf, PACKET_WIDTH );
            hSMP_FRG = ((int)buf[0] << 8) + (int)buf[1];
            hCH_TOP  = (int)buf[2];
            hCH_STP  = (int)buf[3];
            
            // std::cout << "SMP_FRG: " << std::dec << hSMP_FRG << std::endl;
            // std::cout << "CH_TOP : " << std::dec << hCH_TOP  << std::endl;
            // std::cout << "CH_STP : " << std::dec << hCH_STP  << std::endl;

            ifs.read( buf, PACKET_WIDTH );
            hCH_NUM  = (int)buf[0];
            hDT_LEN  = (int)buf[1];
            hREC_LEN = ((int)buf[2] << 8) + (int)buf[3];
            
            // std::cout << "CH_NUM : " << std::dec << hCH_NUM  << std::endl;
            // std::cout << "DT_LEN : " << std::dec << hDT_LEN  << std::endl;
            // std::cout << "REC_LEN: " << std::dec << hREC_LEN << std::endl;

            ifs.read( buf, PACKET_WIDTH );
            // std::cout << "TRIG_TIM1 : " << std::hex << (int)buf[0] << (int)buf[1] << (int)buf[2] << (int)buf[3]  << std::endl;
            // std::cout << "TRIG_TIM2 : " << std::hex << (int)buf[0] << (int)buf[1] << (int)buf[2] << (int)buf[3]  << std::endl;

            // std::cout << std::dec;
            for( int ch = 0; ch < hCH_NUM; ++ch ) {
                ifs.read( adc, (int)(hREC_LEN * sizeof(short)) );
                for( int clk = 0; clk < hREC_LEN; ++clk ) {
                    fadcVar[ch][clk] = ((int)adc[2*clk] << 4 ) + ((int)adc[2*clk+1] >> 4 );
                }
            }
            pTree->Fill( );
            ++trigger;
            std::cout << ".";
        }
    }

    pTree->Write( );

    std::cout << "Decode end." << std::endl;
    std::cout << "Total: " << trigger << "events processed." << std::endl;
    std::cout << pTree->GetEntries( ) << "events are saved to ROOT file." << std::endl;
    std::cout << "Bye..." << std::endl;
    
    file.Close( );
    
    return 0;
}
