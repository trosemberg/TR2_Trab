#include <socket.hpp>
#include <servidor.hpp>
#include <util.hpp>

int Servidor::init(){
    int porta;
    int socket_bind , socket_connect;
    int valread = 0;
	std::string message;
    porta = this->porta;
    //Create socket
    socket_bind = socket.SocketCreate();
    if (socket_bind == -1){
        printf("Could not create socket");
        return 1;
    }
    printf("Socket created\n");
    //Bind
    socket_bind = socket.SocketBind(socket_bind, porta);
    if( socket_bind < 0){
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
    socket_connect = socket.SocketConnect(socket_connect,porta);
    if (socket_connect < 0){
        perror("connect failed.\n");
        return 1;
    }
    
    printf("Sucessfully conected with server\n");
    while(true){
        socket.SocketAccept(socket_bind);
        	std::string message;

        do {
            char buffer[1024];
            valread = static_cast<int>(read(socket.inSocket, buffer, sizeof( buffer ) ));
            message += std::string(buffer, static_cast<unsigned long>(valread));
        } while (valread == 1024);

        if(valread > 0) {
            std::cout<< message;
        } else if( 0 == valread ) {
            printf("\nNão há requests para serem lidos\n");
        } else {
            printf( "\nFalha na leitura de dados\n" );
        }
    }
}

Servidor::Servidor( long int porta){
    this->porta = porta;
}
Servidor::~Servidor()= default;