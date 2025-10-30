#ifndef __tcp_client_h_
#define __tcp_client_h_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

typedef struct
{
    int fd;
    struct sockaddr_in addr;
} TCPClient;

int TCPClient_Initiate(TCPClient* _Client, int _fd);
int TCPClient_Connect(TCPClient* _Client, const char* _Host, const char* _Port);
int TCPClient_Write(TCPClient* _Client, const unsigned char* _Buf, int _Len);
int TCPClient_Read(TCPClient* _Client, unsigned char* _Buf, int _Cap);
void TCPClient_Dispose(TCPClient* _Client);

#endif // __tcp_client_h_
