#include <socket.hpp>
#include <servidor.hpp>
#include <util.hpp>

int Servidor::init(){
    Socket socket;
    int porta;
    int hSocket, read_size;
    struct sockaddr_in server;
    char SendToServer[100] = {0};
    char server_reply[200] = {0};
    porta = this->porta;
    
    //Create socket
    hSocket = socket.SocketCreate();
    if(hSocket == -1)
    {
    printf("Could not create socket\n");
    return 1;
    }
    
    printf("Socket is created\n");
    
    //Connect to remote server
    if (socket.SocketConnect(hSocket, porta) < 0)
    {
    perror("connect failed.\n");
    return 1;
    }
    
    printf("Sucessfully conected with server\n");
        
    
    close(hSocket);
    shutdown(hSocket,0);
    return 0;
}

Servidor::Servidor( long int porta){
    this->porta = porta;
}
Servidor::~Servidor()= default;