#ifndef __SiTCP_H
#define __SiTCP_H

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

class SiTCP
{
public:
    SiTCP( );
    SiTCP( const std::string& ip );
    ~SiTCP( );

    bool Open( const std::string& ip );
    void Close( );

    int Read( char* pData );
    int Read( char* pData, const int& datasize );
    int Read( char* pData, const int& datasize, const int& timeiout_sec );

private:
    struct sockaddr_in m_param;
    int m_socket;
    int m_num_device;
};

#endif // __SiTCP_H
