#include "socket.hpp"
#include "header.hpp"
#include "util.hpp"

#define PORTA 8228

// caso seja fornecida uma porta, usa ela, caso nao usar porta padrao 8228

int main(int argc, char const *argv[]){
	Socket socket((long int)8228);
	HTTP http((long int)8228);
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
	while(1){
		socket.SocketAccept();
		http.RequestHTTP((void*)&socket.inSocket);      
	}
	close(socket.inSocket);
	close(socket.bind_socket);
	printf("Bind e Listen feito \n");

	return 0;
}
