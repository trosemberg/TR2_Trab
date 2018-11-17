#ifndef SOCKET
#define SOCKET 
#include <util.hpp>

class Socket{
public:
    Socket();
    ~Socket();
    int SocketConnect(int hSocket, int porta);
    short SocketCreate();
};

#endif