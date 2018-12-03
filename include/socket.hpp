#ifndef SOCKET
#define SOCKET 
#include <util.hpp>
#include <header.hpp>
class Socket{
public:
    int inSocket = -1;
    int bind_socket = -1;
    int connect_socket = -1;
    Socket(long int porta);
    ~Socket();
    int SocketCreate();
    int SocketBind();
    int SocketConnect();
    int SocketAccept();
private:
    int porta;
};

#endif