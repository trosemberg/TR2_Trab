#include "socket.hpp"
#include "servidor.hpp"
#include "util.hpp"
#include "header.hpp"

// funcao responsavel por rodar o programa todo

int Servidor::init(){
    //Cria socket
    socket.bind_socket = socket.SocketCreate();
    if (socket.bind_socket == -1){
        printf("Could not create socket");
        return 1;
    }
    printf("Socket created\n");
    //Faz o Bind e o Listen do socket
    socket.SocketBind();
    if( socket.bind_socket < 0){
        //print the error message
        perror("bind failed.");
        return 1;
    }

    printf("Bind e Listen feito \n");
}

int Servidor::run(){
        // roda o programa, fazendo o accept e toda a parte de manipulacacao HTTP e Spider
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
