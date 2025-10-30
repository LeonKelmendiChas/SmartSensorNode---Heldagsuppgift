#include <stdio.h>
#include <string.h>
#include "http_post.h"

static int HTTPPost_BuildRequest(
    const char* _Host,
    const char* _Path,
    const char* _Body,
    char* _Out,
    int _Cap)

{
    size_t blen = strlen(_Body);
    int n = snprintf(_Out, (size_t)_Cap,
        "POST %s HTTP/1.1\r\n"
        "Host: %s\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: %zu\r\n"
        "Connection: close\r\n"
        "\r\n"
        "%s",
        _Path, _Host, blen, _Body);

    return (n < 0 || n >= _Cap) ? -1 : 0;
}

int HTTPPost_Initiate(HTTPPost* _Post)
{
    if(!_Post) return -1;

    memset(_Post, 0, sizeof(*_Post));
    _Post->tcpClient.fd = -1;
    _Post->context = NULL;
    _Post->onData  = NULL;
    _Post->recvlen = 0;

    return 0;
}

int HTTPPost_InitiatePtr(HTTPPost** _PostPtr)
{
    if(!_PostPtr) return -1;

    HTTPPost* p = (HTTPPost*)malloc(sizeof(HTTPPost));
    if(!p) return -2;

    int r = HTTPPost_Initiate(p);
    if(r != 0) { free(p); return r; }

    *_PostPtr = p;
    return 0;
}

void HTTPPost_SetCallback(HTTPPost* _Post, void* _Context, HTTPPost_OnData _OnData)
{
    if(!_Post) return;
    _Post->context = _Context;
    _Post->onData  = _OnData;
}

int HTTPPost_Send(HTTPPost* _Post,
                  const char* _Host,
                  const char* _Port,
                  const char* _Path,
                  const char* _JsonBody)
{
    if(!_Post || !_Host || !_Port || !_Path || !_JsonBody)
        return -1;

    char req[1024];
    if(HTTPPost_BuildRequest(_Host, _Path, _JsonBody, req, (int)sizeof(req)) != 0)
        return -2;

    printf("Connecting to %s:%s\n", _Host, _Port);

    if(TCPClient_Initiate(&_Post->tcpClient, -1) != 0)
        return -3;

    if(TCPClient_Connect(&_Post->tcpClient, _Host, _Port) != 0) {
        printf("Connection failed\n");
        TCPClient_Dispose(&_Post->tcpClient);
        return -4;
    }

    printf("Sending...\n");
    int w = TCPClient_Write(&_Post->tcpClient,
                            (const unsigned char*)req,
                            (int)strlen(req));
    if(w < 0) {
        printf("Send failed\n");
        TCPClient_Dispose(&_Post->tcpClient);
        return -5;
    }

    char buf[512];
    for(;;) {
        int n = TCPClient_Read(&_Post->tcpClient, (unsigned char*)buf, (int)sizeof(buf));
        if(n > 0) {
            if(_Post->onData)
                _Post->onData(_Post->context, buf, n);
            continue;
        }
        if(n == 0) break;
        if(n < 0)  break;
    }

    TCPClient_Dispose(&_Post->tcpClient);
    printf("Done\n");
    return 0;
}

void HTTPPost_Dispose(HTTPPost* _Post)
{
    if(!_Post) return;
    TCPClient_Dispose(&_Post->tcpClient);
}

void HTTPPost_DisposePtr(HTTPPost** _PostPtr)
{
    if(!_PostPtr || !*_PostPtr) return;
    HTTPPost_Dispose(*_PostPtr);
    free(*_PostPtr);
    *_PostPtr = NULL;
}
