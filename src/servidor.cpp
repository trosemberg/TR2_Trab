#include <socket.hpp>
#include <servidor.hpp>
#include <util.hpp>

int Servidor::init(){
    int porta;
    int hSocket;
    struct sockaddr_in server;
    porta = (int)this->porta;
    
    //Create socket
    hSocket = socket.SocketCreate();
    if(hSocket == -1)
    {
    printf("Could not create socket\n");
    return 1;
    }
    
    printf("Socket is created\n");
    
    //Connect to remote server
    if (socket.SocketBind(hSocket, porta) < 0)
    {
    perror("connect failed.\n");
    return 1;
    }
    
    printf("Sucessfully conected with server\n");
    return 0;
}

Servidor::Servidor( long int porta){
    this->porta = porta;
}
Servidor::~Servidor()= default;