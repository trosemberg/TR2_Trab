#ifndef SOCKET
#define SOCKET 
#include <util.hpp>

class Socket{
public:
    Socket();
    ~Socket();
    int SocketConnect(int hSocket, int porta);
    int SocketSend(int hSocket,char* Rqst,short lenRqst);
    short SocketCreate();
};

#endif