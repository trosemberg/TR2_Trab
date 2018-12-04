#include "socket.hpp"
#include "servidor.hpp"
#include "util.hpp"

Socket::Socket(long int porta) {
    this->porta = (int)porta;
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
int Socket::createServerSocket(char *requestHost, char *requestPort) {
  
  struct addrinfo host_info;
  struct addrinfo *host_info_list;
  int idSocket;
  //Obtem informações de endereço para o soquete de fluxo na porta de entrada
  memset(&host_info, 0, sizeof(host_info));
  host_info.ai_family = AF_UNSPEC;
  host_info.ai_socktype = SOCK_STREAM;
  if (getaddrinfo(requestHost, requestPort, &host_info, &host_info_list) != 0) {
   		fprintf(stderr," Erro no formato do endereco do servidor!\n");
		exit (1);
  }
  //cria um socket
  if ((idSocket = socket(host_info_list->ai_family, host_info_list->ai_socktype, host_info_list->ai_protocol)) < 0) 
  {
    	fprintf(stderr," Erro ao criar socket para o servidor!\n");
		exit (1);
  }
  //faz a conecção
  if (connect(idSocket, host_info_list->ai_addr, host_info_list->ai_addrlen) < 0)
  {
    	fprintf(stderr," Erro ao tentar conectar o servidor!\n");
		exit (1);
  }
  freeaddrinfo(host_info_list);
  return idSocket;
}

void Socket::sendToServerSocket(const char* bufferServer,int socketfd,int sizeBuffer)
{

	std::string temp;

	temp.append(bufferServer);
	
	int totalSent = 0;

	int numSent;

	while (totalSent < sizeBuffer) {
		if ((numSent = send(socketfd, (void *) (bufferServer + totalSent), sizeBuffer - totalSent, 0)) < 0) {
			fprintf(stderr," Erro ao enviar para o servidor!\n");
			exit (1);
		}
		totalSent += numSent;

	}	

}
void Socket::sendToClientSocket(const char* bufferServer,int socketfd,int sizeBuffer)
{
	std::string temp;

	temp.append(bufferServer);
	
	int totalSent = 0;

	int numSent;

	while (totalSent < sizeBuffer) {
		if ((numSent = send(socketfd, (void *) (bufferServer + totalSent), sizeBuffer - totalSent, 0)) < 0) {
			fprintf(stderr," Erro ao receber do servidor!\n");
			exit (1);
		}
		totalSent += numSent;

	}	
}
void Socket::receiveFromServer (int Clientfd, int Serverfd) 
{
	int sizeBuffer = 5000;
	int iRecv;
	char buffer[sizeBuffer];
	/**
		para receber a resposta HTTP do servidor remoto eu 
		crio um laço while que verifica se a quantidade de 
		bytes recebidos é maior que zero
			caso em que iRecv > 0, armazeno o que eu recebi em um buffer
								   e repasso imediatamente para o browser
			caso em que o iRecv == 0, significa que o servidor remoto
									  terminou de enviar toda a resposta HTTP
			caso em que o iRecv < 0, significa um erro e o programa é encerrado
	**/
	while ((iRecv = recv(Serverfd, buffer, sizeBuffer, 0)) > 0) {
	    sendToClientSocket(buffer, Clientfd,iRecv);         // writing to client	  
		memset(buffer,0,sizeof (buffer));	
	}      
	if (iRecv < 0) {
	  fprintf(stderr,"Erro enquanto recebia do servidor!\n");
	  exit (1);
	}
}