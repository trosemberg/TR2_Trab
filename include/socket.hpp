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
    int SocketCreate();
    int SocketBind(int porta);
    int SocketConnect(int porta);
    int SocketAccept();
    ssize_t SendOut( HTTP::Header msg );
};

#endif