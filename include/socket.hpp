#ifndef SOCKET
#define SOCKET 
#include <util.hpp>

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
    int createServerSocket(char *requestHost, char *requestPort);
    void sendToServerSocket(const char* bufferServer,int socketfd,int sizeBuffer);
    void sendToClientSocket(const char* bufferServer,int socketfd,int sizeBuffer);
    void receiveFromServer (int Clientfd, int Serverfd);
private:
    int porta;
};

#endif