#ifndef SOCKET
#define SOCKET 
#include <util.hpp>

class Socket{
public:
    Socket();
    ~Socket();
    short SocketCreate();
    int SocketBind(int hSocket, int porta);
};

#endif