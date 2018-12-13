#include "socket.hpp"
#include "util.hpp"

Socket::Socket() = default;
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
    remote.sin_port = htons(8228);
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
//    if(inSocket < 0) {
//        perror("accept failed.");
//    }
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
  //faz a conexao
  if (connect(idSocket, host_info_list->ai_addr, host_info_list->ai_addrlen) < 0)
  {
        throw(" Erro ao tentar conectar o servidor!\n");
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
std::string Socket::sendToClientSocket(const char* bufferServer,int socketfd,int sizeBuffer)
{
    std::string temp,str2;
    std::size_t found;

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
    found = temp.find("\r\n\r\n");
    if(found!=std::string::npos){
        str2 = temp.substr(0,found);
        return str2;
    }
    return "";
}

// recebe de volta do servidor a resposta
std::string Socket::receiveFromServer (int Clientfd, int Serverfd) {
    std::string temp = "";
    int sizeBuffer = 5000;
    int iRecv;
    char buffer[sizeBuffer];
        // while de verificacao se o recebido e maior que 0 bits
        // iRecv > 0 armazena o buffer e =0 terminou de enviar
    std::cout<<"\n\n\e[0m A response está salva no arquivo \e[92mresponse\e[0m\n";
    std::cout<<"\n\e[92mResposta do servidor:\n";
    while ((iRecv = recv(Serverfd, buffer, sizeBuffer, 0)) > 0) {
        temp = sendToClientSocket(buffer, Clientfd,iRecv);         // writing to client
        memset(buffer,0,sizeof (buffer));
    }
    if (iRecv < 0) {
      throw("Erro enquanto recebia do servidor!\n");
    }
    return temp;
}
