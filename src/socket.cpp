#include <socket.hpp>
#include <servidor.hpp>
#include <util.hpp>

Socket::Socket(long int porta) {
    this->porta = porta;
}
Socket::~Socket() = default;

//Create a Socket for server communication
int Socket::SocketCreate()
{ 
    int hSocket;
    printf("\nCreate the socket\n");
    hSocket = socket(AF_INET, SOCK_STREAM, 0);
    return hSocket;
}


//try to bind socket with server
int Socket::SocketBind()
{
    struct sockaddr_in remote;

    remote.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
    remote.sin_family = AF_INET;
    remote.sin_port = htons(porta);
    if( bind(bind_socket , (struct sockaddr *)&remote , sizeof(struct sockaddr_in)) < 0) {
		printf("\nErro no bind do socket\n");
		return -1;
	}
	listen(bind_socket,10);

	printf("\nSocket binded\n");


    return bind_socket;
}

int Socket::SocketConnect(){
    struct sockaddr_in remote;

    remote.sin_addr.s_addr = inet_addr("127.0.0.1"); //Local Host
    remote.sin_family = AF_INET;
    remote.sin_port = htons(porta);
    if(connect(connect_socket , (struct sockaddr *)&remote , sizeof(struct sockaddr_in)) < 0) {
		perror("\nErro no connect do socket");
		return -1; 
	}

    return connect_socket;
}
int Socket::SocketAccept(){
    inSocket = accept(bind_socket, nullptr, nullptr);
	if(inSocket < 0) {
		perror("accept failed.");
	}
    return 0;
}
