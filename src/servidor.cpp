#include <socket.hpp>
#include <servidor.hpp>
#include <util.hpp>

int Servidor::init(){
    int porta;
    int socket_bind , socket_connect;
    porta = this->porta;
    //Create socket
    socket_bind = socket.SocketCreate();
    if (socket_bind == -1){
        printf("Could not create socket");
        return 1;
    }
    printf("Socket created\n");
    //Bind
    if( socket.SocketBind(socket_bind, porta) < 0){
        //print the error message
        perror("bind failed.");
        return 1;
    }
    printf("bind done \n");
    socket_connect = socket.SocketCreate();
    if(socket_connect == -1){
        printf("Could not create socket\n");
    return 1;
    }
    
    printf("Socket is created\n");
    
    //Connect to remote server
    if (socket.SocketConnect(socket_connect,porta) < 0){
        perror("connect failed.\n");
        return 1;
    }
    
    printf("Sucessfully conected with server\n");
}

Servidor::Servidor( long int porta){
    this->porta = porta;
}
Servidor::~Servidor()= default;