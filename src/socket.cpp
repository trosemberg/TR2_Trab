#include <socket.hpp>
#include <servidor.hpp>
#include <util.hpp>
Socket::Socket() = default;
Socket::~Socket() = default;

//Create a Socket for server communication
short Socket::SocketCreate()
{ 
    short hSocket;
    printf("\nCreate the socket\n");
    hSocket = socket(AF_INET, SOCK_STREAM, 0);
    return hSocket;
}


//try to bind socket with server
int Socket::SocketBind(int hSocket, int porta)
{
    socklen_t size;
    struct sockaddr_in remote;

    remote.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
    remote.sin_family = AF_INET;
    remote.sin_port = htons(porta);
    bind_socket = bind(hSocket , (struct sockaddr *)&remote , sizeof(struct sockaddr_in));
    if( bind_socket < 0) {
		printf("\nErro no bind do socket\n");
		return -1;
	}
	listen(hSocket,10);

	printf("\nSocket binded\n");
	printf("\nProcurando cliente...\n");


    return bind_socket;
}

int Socket::SocketConnect(int hSocket, int porta){
    struct sockaddr_in remote;

    remote.sin_addr.s_addr = inet_addr("127.0.0.1"); //Local Host
    remote.sin_family = AF_INET;
    remote.sin_port = htons(porta);
    connect_socket = connect(hSocket , (struct sockaddr *)&remote , sizeof(struct sockaddr_in));
    if(connect_socket < 0) {
		printf("\nErro no connect do socket\n");
		return -1; 
	}

    return connect_socket;
}
int Socket::SocketAccept(){
    inSocket = accept(bind_socket, nullptr, nullptr);
	if(inSocket < 0) {
		printf("\nErro na aceitação de conexão\n");
		exit(1);
	}
    return 0;
}
