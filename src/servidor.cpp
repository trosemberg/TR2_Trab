#include <socket.hpp>
#include <servidor.hpp>
#include <util.hpp>
#include <header.hpp>

int Servidor::init(){
    int valread = 0;
	std::string message;
    //Create socket
    socket.bind_socket = socket.SocketCreate();
    if (socket.bind_socket == -1){
        printf("Could not create socket");
        return 1;
    }
    printf("Socket created\n");
    //Bind
    socket.SocketBind();
    if( socket.bind_socket < 0){
        //print the error message
        perror("bind failed.");
        return 1;
    }

    printf("bind done \n");
    socket.connect_socket = socket.SocketCreate();
    if(socket.connect_socket == -1){
        printf("Could not create socket\n");
    return 1;
    }
    
    printf("Socket is created\n");

    
    printf("Sucessfully conected with server\n\n");
    while(true){
        socket.inSocket = accept(socket.bind_socket, nullptr, nullptr);
	    if(socket.inSocket < 0) {
		    perror("accept failed.");
	    }
        std::string message;
        //Connect to remote server

        do {
            char buffer[1024];
            valread = static_cast<int>(read(socket.inSocket, buffer, sizeof( buffer ) ));
            message += std::string(buffer, static_cast<unsigned long>(valread));
	    } while (valread == 1024);

	    if(valread > 0) { 
		    requestsRcv.push_back(HTTP::Header(message));
	    } else if( 0 == valread ) {
		    printf("\nNão há requests para serem lidos\n");
	    } else {
		    printf( "\nFalha na leitura de dados\n" );
	    }
        std::cout<<"\n\n\n"<<message;
        socket.SocketConnect();
        if (socket.connect_socket < 0){
            perror("connect failed.\n");
        return 1;
        }
    }
}

Servidor::Servidor(long int porta) :  requestsRcv(socket.requestsRcv),
							        responsesRcv(socket.responsesRcv),
                                    socket(porta) {}
                            
Servidor::~Servidor()= default;