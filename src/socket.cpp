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

void Socket::GetRequest(){
    int valread = 0;
	std::string message;
    do {
            char buffer[1024];
            valread = static_cast<int>(read(inSocket, buffer, sizeof( buffer ) ));
            message += std::string(buffer, static_cast<unsigned long>(valread));
	    } while (valread == 1024);

	    if(valread > 0) { 
		    requestsRcv.push_back(HTTP::Header(message));
	    } else if( 0 == valread ) {
		    printf("\nNão há requests para serem lidos\n");
	    } else {
		    printf( "\nFalha na leitura de dados\n" );
	    }
}

void Socket::AnswerRequest(){
    int valread = 0;
	std::string message("");

	do {
		char buffer[1024];
		valread = static_cast<int>(read(connect_socket, buffer, sizeof( buffer ) ));
		message += std::string(buffer, static_cast<unsigned long>(valread));
	} while (valread == 1024);

	if(valread > 0) {
		responsesRcv.push_back(HTTP::Header(message));
	} else {
		printf( "\nFalha na leitura de dados\n" );
		exit(1);
	}
}

ssize_t Socket::SendOut(HTTP::Header msg){
    struct addrinfo client_addr;
    struct addrinfo *addr_c;
	client_addr.ai_family = AF_INET;
	client_addr.ai_socktype = INADDR_ANY;
	client_addr.ai_flags = 0;
	client_addr.ai_protocol = 0;

	int info = getaddrinfo( msg.host.c_str(), msg.port.c_str(), &client_addr, &addr_c);
	if(info == 0){
		connect_socket = socket(AF_INET, SOCK_STREAM, 0);
		if(connect_socket < 0){
			printf("\nErro ao criar out socket\n");
			exit(1);
		} 
		if(connect(connect_socket, addr_c->ai_addr, addr_c->ai_addrlen) < 0){
			printf("\nErro na conexão do socket\n");
			exit(1);
		}
	}

	ssize_t sent = send(connect_socket, msg.to_string().c_str(), msg.to_string().length(), 0);

	if(sent < 0){
		printf("\nNão foi possível enviar dado\n");
		exit(1);
	} else {
		return sent;
	}

	return -1;
}

ssize_t Socket::SendIn(HTTP::Header msg){
	ssize_t sent = send(bind_socket, msg.to_string().c_str(), msg.to_string().length(), 0);
	if(sent < 0){
		printf("\nNão foi possível enviar dado\n");
		exit(1);
	} else {
		return sent;
	}
}