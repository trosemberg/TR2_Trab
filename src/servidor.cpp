#include <socket.hpp>
#include <servidor.hpp>
#include <util.hpp>
#include <header.hpp>

int Servidor::init(){
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
    printf("Sucessfully conected with server\n\n");
    while(1){
        socket.SocketAccept();
		http.RequestHTTP((void*)&socket.inSocket);        
    }
	close(socket.inSocket);
 	close(socket.bind_socket);
}

Servidor::Servidor(long int porta) :socket(porta),
                                    http(porta){}
                            
Servidor::~Servidor()= default;