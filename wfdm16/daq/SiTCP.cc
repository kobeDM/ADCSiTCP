#include "SiTCP.h"
#include <unistd.h>

SiTCP::SiTCP()
{
    m_num_device = 0;
    return;
}

SiTCP::SiTCP( const std::string& ip )
{
    m_num_device = 0;
    Open( ip );
    return;
}

SiTCP::~SiTCP( )
{
    if( m_num_device ) Close( );
}

bool SiTCP::Open( const std::string& ip )
{
    if( !m_num_device ) {
        m_param.sin_port        = htons( 24 );
        m_param.sin_family      = AF_INET;
        m_param.sin_addr.s_addr = inet_addr( ip.c_str( ) );

        m_socket = socket( AF_INET, SOCK_STREAM, 0 );
        if( m_socket < 0 ) {
            std::cerr << " ERROR(SiTCP): Can't open socket" << std::endl;
            return 1;
        }
        if( connect( m_socket, ( struct sockaddr* )&m_param, sizeof( m_param ) ) ) {
            std::cerr << " ERROR(SiTCP): Can't connet" << std::endl;
            return 1;
        }

        m_num_device++;
    }

    return 0;
}

void SiTCP::Close( )
{
    if( m_num_device ) close( m_socket );
}

int SiTCP::Read( char* pData )
{
    return Read( pData, 4096 );
}

int SiTCP::Read( char *pData, const int& datasize )
{
    return recv(m_socket, pData, datasize, 0);
}


int SiTCP::Read( char *pData, const int& datasize, const int& timeout_sec )
{
    // enable non-blocking mode
    int val = 1;
    ioctl( m_socket, FIONBIO, &val );

    int len = 0;
    time_t start_time = time( nullptr );
    while( 1 ) {
        if( time( nullptr ) - start_time > timeout_sec ) {
            printf( "Timeout\n" );
            return len;
        }

        len = recv( m_socket, pData, 4096, 0 );
        if( len < 0 ) {
            if( errno == EAGAIN ) {
                usleep(1000000);
            }
            else {
                perror( "recv" );
                break;
            }
        }
        else {
            break;
        }
    }
    return len;
}

