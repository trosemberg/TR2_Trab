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
// cria socket de server
int Socket::createServerSocket(char *requestHost, char *requestPort) {
  
  struct addrinfo host_info;
  struct addrinfo *host_info_list;
  int idSocket;
  //Obtem informações de endereço para o soquete de fluxo na porta de entrada
  memset(&host_info, 0, sizeof(host_info));
  host_info.ai_family = AF_UNSPEC;
  host_info.ai_socktype = SOCK_STREAM;
  if (getaddrinfo(requestHost, requestPort, &host_info, &host_info_list) != 0) {
   		throw(" Erro no formato do endereco do servidor!\n");
  }
  //cria um socket
  if ((idSocket = socket(host_info_list->ai_family, host_info_list->ai_socktype, host_info_list->ai_protocol)) < 0) 
  {
    	throw(" Erro ao criar socket para o servidor!\n");
  }
  //faz a conecção
  if (connect(idSocket, host_info_list->ai_addr, host_info_list->ai_addrlen) < 0)
  {
    	throw(" Erro ao tentar conectar o servidor!\n");
		exit (1);
  }
  freeaddrinfo(host_info_list);
  return idSocket;
}
// envia a request por meio do socket criado
void Socket::sendToServerSocket(const char* bufferServer,int socketfd,int sizeBuffer)
{

	std::string temp;

	temp.append(bufferServer);
	
	int totalSent = 0;

	int numSent;

	while (totalSent < sizeBuffer) {
		if ((numSent = send(socketfd, (void *) (bufferServer + totalSent), sizeBuffer - totalSent, 0)) < 0) {
			throw(" Erro ao enviar para o servidor!\n");
		}
		totalSent += numSent;

	}	

}
// envia a http recebida para o browser
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
	std::cout<<temp;	
}

// recebe de volta do servidor a resposta
// TODO FAZER RESPONSE SER EDITAVEL
void Socket::receiveFromServer (int Clientfd, int Serverfd) {
	std::string temp;
	int sizeBuffer = 5000;
	int iRecv;
	char buffer[sizeBuffer];
	std::fstream response;
	response.open("response", std::ios::binary| std::ios::out);
		// while de verificacao se o recebido e maior que 0 bits
		// iRecv > 0 armazena o buffer e =0 terminou de enviar
		std::cout<<"\n\e[92mResposta do servidor:\n";
	while ((iRecv = recv(Serverfd, buffer, sizeBuffer, 0)) > 0) {
		temp.append(buffer);
	    sendToClientSocket(buffer, Clientfd,iRecv);         // writing to client	  
		memset(buffer,0,sizeof (buffer));	
	}
	response<< temp;
	response.close();     
	if (iRecv < 0) {
	  throw("Erro enquanto recebia do servidor!\n");
	}
}
