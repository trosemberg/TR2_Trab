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
    void GetRequest();
    void AnswerRequest();
    ssize_t SendOut( HTTP::Header msg );
    ssize_t SendIn( HTTP::Header msg );
    std::vector< HTTP::Header > requestsRcv;
    std::vector< HTTP::Header > responsesRcv;
private:
    int porta;
};

#endif