#include "inc/shinclude.h"
#include "SiTCP.h"
// #include <iomanip>

const int    NUM_CH       = 8;
const size_t PACKET_WIDTH = 4; // 32 bit per word
const int    MAX_SAMPLING = 4096;
const String ADCSiTCP_IP  = "192.168.10.16";
const int    HEADER_SIZE  = 20; // 20 Byte
const int    ADC_RANGE    = 16; // Actual ADC resolution is 12 bit
const int    PACKET_SIZE  = ADC_RANGE * MAX_SAMPLING * NUM_CH / 8 + HEADER_SIZE;

int usage()
{
    ShUtil::Cinfo( "Usage: ./daq [IP] [output filename] [number of events]" );
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc != 4) {
        usage();
        exit(1);
    }
    
    String sitcp_ip    = argv[1];
    String outfilename = argv[2];
    int    nEvent      = std::stoi( argv[3] );

    std::cout << "--- ADC SiTCP DAQ START ---" << std::endl;
    std::cout << std::endl;
    std::cout << "- Configuration -" <<  std::endl;
    std::cout << "IP: " << ADCSiTCP_IP << std::endl;
    std::cout << "#Channels: " << NUM_CH << std::endl;
    std::cout << "SAMPLING: " << MAX_SAMPLING << std::endl;
    std::cout << "#Events: " << nEvent << std::endl;
    std::cout << "Output filename: " << outfilename << std::endl;

    std::cout << std::endl;

    std::cout << "- Trying to establish TCP connection... -" << std::endl;
    SiTCP SiTCP;
    if( SiTCP.Open( sitcp_ip ) != 0 ) {
        ShUtil::Cerr( "Failed to connect to ADCSiTCP..." );
        exit(0);
    }

    std::cout << "Connection established to " << ADCSiTCP_IP << std::endl;

    std::ofstream ofs;
    ofs.open( outfilename, std::ios::out );

    std::time_t starttime = std::time( nullptr );
    
    std::cout << std::endl;
    std::cout << "DAQ START: " << starttime << "\t" << std::ctime( &starttime ) << std::endl;

    std::cout << std::endl;
    std::cout << "Event store: " << nEvent << " events." << std::endl;

    int  evtIdx     = 0;
    char* pData = new char[ PACKET_SIZE ];
    int getByteSize = 0, totalByteSize = 0;
    String evtText = "";
    std::cout << "Events: " << evtIdx << " / " << nEvent << std::endl;
    while( 1 ) {
        getByteSize = SiTCP.Read( pData, PACKET_SIZE );
        if( getByteSize > 0 ) {
            ofs.write( pData, getByteSize );
            totalByteSize += getByteSize;

            if( totalByteSize >= PACKET_SIZE ) {
                ++evtIdx;
                totalByteSize = 0;
                if( evtIdx % 10 == 0 )
                    std::cout << "Events: " << evtIdx << " / " << nEvent << std::endl;
            }
        }

        if( evtIdx >= nEvent ) break;
    }

    std::time_t endtime = std::time( nullptr );
    std::cout << std::endl;
    std::cout << "DAQ END: " << endtime << "\t" << std::ctime( &endtime ) << std::endl;
    std::cout << "Total: " << (double)nEvent / (double)( endtime - starttime ) << " Hz" << std::endl;

    
    ofs.close( );
    
    return 0;
}
