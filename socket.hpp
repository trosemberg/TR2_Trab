#ifndef SOCKET
#define SOCKET
#include "util.hpp"

// Classe responsavel pelas funções que envolvem socket
class Socket{
public:
    int inSocket = -1;
    int bind_socket = -1;
    int connect_socket = -1;
    int porta = 8228;
    Socket();
    ~Socket();
    int SocketCreate();
    int SocketBind();
    int SocketConnect();
    int SocketAccept();
    int createServerSocket(char *requestHost, char *requestPort);
    void sendToServerSocket(const char* bufferServer,int socketfd,int sizeBuffer);
    std::string sendToClientSocket(const char* bufferServer,int socketfd,int sizeBuffer);
    std::string receiveFromServer (int Clientfd, int Serverfd);
};

#endif
