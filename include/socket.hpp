#ifndef SOCKET
#define SOCKET 
#include <util.hpp>

class Socket{
public:
    int inSocket = -1;
    int bind_socket = -1;
    int connect_socket = -1;
    Socket();
    ~Socket();
    short SocketCreate();
    int SocketBind(int hSocket, int porta);
    int SocketConnect(int hSocket, int porta);
    int SocketAccept(int hSocket);
};

#endif