#include "inc/shinclude.h"
#include "SiTCP.h"
// #include <iomanip>

const int    NUM_CH       = 8;
const size_t PACKET_WIDTH = 4; // 32 bit per word
const int    MAX_SAMPLING = 4096;
const String ADCSiTCP_IP  = "192.168.10.16";
const int    HEADER_SIZE  = 20; // 20 Byte
const int    ADC_RANGE    = 16; // Actual ADC resolution is 12 bit
const int    PACKET_SIZE  = ADC_RANGE * MAX_SAMPLING * NUM_CH;

int usage()
{
    ShUtil::Cinfo( "Usage: ./daq [IP] [output filename] [number of events]" );
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
        usage();
        exit(1);
    }
    
    String sitcp_ip    = argv[1];
    String outfilename = argv[2];
    int    nEvent      = std::stoi( argv[3] );
    
    SiTCP SiTCP;
    if( SiTCP.Open( sitcp_ip ) != 0 ) {
        ShUtil::Cerr( "Failed to connect to ADCSiTCP..." );
        exit(0);
    }

    std::ofstream ofs;
    ofs.open( outfilename, std::ios::out );
    
    int  evtIdx     = 0;
    char* pData = new char[ PACKET_SIZE ];
    while( 1 ) {
        int num = SiTCP.Read( pData, PACKET_SIZE );
        if( num > 0 ) {
            ofs.write( pData, num );
            ++evtIdx;
        }
        std::cout << "Event: " << evtIdx << "/" << nEvent << std::endl;

        if( evtIdx >= nEvent ) break;
    }
    ofs.close( );
    
    return 0;
}
