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

//try to connect with server
int Socket::SocketConnect(int hSocket, int porta)
{
    socklen_t size;
    struct sockaddr_in remote;

    remote.sin_addr.s_addr = htons(INADDR_ANY); //Local Host
    remote.sin_family = AF_INET;
    remote.sin_port = htons(static_cast<uint16_t>(porta));

    
    if(bind(hSocket, (struct sockaddr*)&remote, sizeof(remote)) < 0) {
		printf("\nErro no bind do socket\n");
		return -1;
	}

	printf("\nSocket binded\n");
	size = sizeof(remote);
	printf("\nProcurando cliente...\n");

	listen(hSocket,10);


    return 0;
}

