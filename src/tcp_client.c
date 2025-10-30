#include "tcp_client.h"

int TCPClient_Initiate(TCPClient* _Client, int _fd)
{
    if (!_Client) return -1;
    memset(_Client, 0, sizeof(*_Client));
    _Client->fd = _fd;
    return 0;
}

int TCPClient_Connect(TCPClient* _Client, const char* _Host, const char* _Port)
{
    if (!_Client || !_Host || !_Port) return -1;

    struct hostent* server = gethostbyname(_Host);
    if (!server) return -2;

    int port = atoi(_Port);
    _Client->fd = socket(AF_INET, SOCK_STREAM, 0);
    if (_Client->fd < 0) return -3;

    memset(&_Client->addr, 0, sizeof(_Client->addr));
    _Client->addr.sin_family = AF_INET;
    _Client->addr.sin_port = htons(port);
    memcpy(&_Client->addr.sin_addr.s_addr, server->h_addr, server->h_length);

    if (connect(_Client->fd, (struct sockaddr*)&_Client->addr, sizeof(_Client->addr)) < 0)
    {
        close(_Client->fd);
        return -4;
    }

    return 0;
}

int TCPClient_Write(TCPClient* _Client, const unsigned char* _Buf, int _Len)
{
    if (!_Client || _Client->fd < 0) return -1;
    return (int)send(_Client->fd, _Buf, _Len, 0);
}

int TCPClient_Read(TCPClient* _Client, unsigned char* _Buf, int _Cap)
{
    if (!_Client || _Client->fd < 0) return -1;
    return (int)recv(_Client->fd, _Buf, _Cap, 0);
}

void TCPClient_Dispose(TCPClient* _Client)
{
    if (!_Client) return;
    if (_Client->fd >= 0)
    {
        close(_Client->fd);
        _Client->fd = -1;
    }
}
