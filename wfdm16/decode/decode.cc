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
    pTree->Branch( "fadc0", fadcVar[0], "fadc0[4096]/I" );
    pTree->Branch( "fadc1", fadcVar[1], "fadc1[4096]/I" );
    pTree->Branch( "fadc2", fadcVar[2], "fadc2[4096]/I" );
    pTree->Branch( "fadc3", fadcVar[3], "fadc3[4096]/I" );
    pTree->Branch( "fadc4", fadcVar[4], "fadc4[4096]/I" );
    pTree->Branch( "fadc5", fadcVar[5], "fadc5[4096]/I" );
    pTree->Branch( "fadc6", fadcVar[6], "fadc6[4096]/I" );
    pTree->Branch( "fadc7", fadcVar[7], "fadc7[4096]/I" );

    std::cout << "Decode start." << std::endl;
    while( !ifs.eof( ) ) {
        ifs.read( buf, PACKET_WIDTH );
        unsigned char ucharbuf[PACKET_WIDTH] = {}; 
        for(int i = 0; i < PACKET_WIDTH; i++ ) ucharbuf[i] = (unsigned char)buf[i];

        // read header
        if( (unsigned int)ucharbuf[0] == 0xaa && (unsigned int)ucharbuf[1] == 0x55 ) {
            hID      = ((unsigned int)ucharbuf[0] << 8) + (unsigned int)ucharbuf[1];
            hTRG_POS = ((unsigned int)ucharbuf[2] << 8) + (unsigned int)ucharbuf[3];

            ifs.read( buf, PACKET_WIDTH );
            for(int i = 0; i < PACKET_WIDTH; i++ ) ucharbuf[i] = (unsigned char)buf[i];
            hSMP_FRG = ((unsigned int)ucharbuf[0] << 8) + (unsigned int)ucharbuf[1];
            hCH_TOP  = (unsigned int)ucharbuf[2];
            hCH_STP  = (unsigned int)ucharbuf[3];
            
            ifs.read( buf, PACKET_WIDTH );
            for(int i = 0; i < PACKET_WIDTH; i++ ) ucharbuf[i] = (unsigned char)buf[i];
            hCH_NUM  = (unsigned int)ucharbuf[0];
            hDT_LEN  = (unsigned int)ucharbuf[1];
            hREC_LEN = ((unsigned int)ucharbuf[2] << 8) + (unsigned int)ucharbuf[3];

            ifs.read( buf, PACKET_WIDTH );
            for(int i = 0; i < PACKET_WIDTH; i++ ) ucharbuf[i] = (unsigned char)buf[i];

            for( int ch = 0; ch < hCH_NUM; ++ch ) {
                ifs.read( adc, (int)(hREC_LEN * sizeof(short)) );
        	unsigned char ucharadc[MAX_SAMPLING*2] = {}; 
	 	for(int i = 0; i < MAX_SAMPLING*2 ; i++ ) ucharadc[i] = (unsigned char)adc[i];		
                for( int clk = 0; clk < hREC_LEN; ++clk ) {
                    fadcVar[ch][clk] = ((unsigned int)ucharadc[2*clk] << 4 ) + ((unsigned int)ucharadc[2*clk+1] >> 4 );
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
