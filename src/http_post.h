#ifndef __http_post_h_
#define __http_post_h_

#include "tcp_client.h"

typedef int (*HTTPPost_OnData)(void* _Context, const char* _Data, int _Len);

typedef struct
{
    TCPClient tcpClient;
    void* context;
    HTTPPost_OnData onData;
    char recvbuf[1024];
    int recvlen;
} HTTPPost;

int HTTPPost_Initiate(HTTPPost* _Post);
int HTTPPost_InitiatePtr(HTTPPost** _PostPtr);
void HTTPPost_SetCallback(HTTPPost* _Post, void* _Context, HTTPPost_OnData _OnData);
int HTTPPost_Send(HTTPPost* _Post, const char* _Host, const char* _Port, const char* _Path, const char* _JsonBody);
void HTTPPost_Dispose(HTTPPost* _Post);
void HTTPPost_DisposePtr(HTTPPost** _PostPtr);

#endif // __http_post_h_
