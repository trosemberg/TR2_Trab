#include "socket.hpp"
#include "header.hpp"
#include "util.hpp"

#define PORTA 8228

// caso seja fornecida uma porta, usa ela, caso nao usar porta padrao 8228

int main(){
	Socket socket((long int)8228);
	HTTP http((long int)8228);
	Crawler crawler;
	std::fstream request;
	int sizeBuffer = 5000;
	int opcao;
	char buffer[sizeBuffer];
	std::string go;
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
	while(1){
		socket.SocketAccept();
		request.open("request",std::ios::out);
		int newsockfd = socket.inSocket;
		int total_de_bits_recebidos = 0, recvd;

		int iServerfd;

		char *mensagem;  // armazena a mensagem da URL

		char *browser_request;

		mensagem = (char *) malloc(sizeBuffer); 

		mensagem[0] = '\0';
		// determina onde é o fim da request
		while (strstr(mensagem, "\r\n\r\n") == NULL) {  

			recvd = recv(newsockfd, buffer, sizeBuffer, 0) ;

			if(recvd < 0 ){
				fprintf(stderr," Erro ao receber mensagem do cliente!\n");
				exit (1);				
			}else if(recvd == 0) {
				break;
			} else {
				total_de_bits_recebidos += recvd;
				//Se o tamanho da mensagem for maior que o tamanho da string buffer, dobra o tamanho da string
				buffer[recvd] = '\0';
				if (total_de_bits_recebidos > sizeBuffer) {
					sizeBuffer *= 2;
					mensagem = (char *) realloc(mensagem, sizeBuffer);
					if (mensagem == NULL) {
						fprintf(stderr," Erro durante a realocação de memoria!\n");
						exit (1);
					}
				}
			}
			strcat(mensagem, buffer);
		}
		std::string msg(mensagem);
		request << mensagem;
		request.close();
		std::cout<<"\nA requisição esta salva no arquivo \e[93mrequest\e[0m\n";
		std::cout<<"para continuar basta apertar qualquer botao\n";
		std::cin>>go;
		request.open("request",std::ios::in);
		msg.assign((std::istreambuf_iterator<char>(request)),
				std::istreambuf_iterator<char>());
		request.close();
		strcpy(mensagem, msg.c_str());
		if(strlen(mensagem) > 0){
			// cria o pedido analisado, struct com as informacoes das requests
			struct PedidoAnalisado *pedido;
			pedido = http.PedidoAnalisado_create();
			std::cout<<"\n----------------------------------------------------\n";
			std::cout<<"\e[93mResquisicao HTTP do browser:\n";
			std::cout<<mensagem<<"\n\e[0m";
			printf("1 - Spider \n2 - QUIT\n3 - >Browser\nDigite a opcao >> ");
			scanf("%d", &opcao);
			// analisa a request
			http.Analise_do_pedido(pedido, mensagem, strlen(mensagem));
			switch(opcao)
			{
				// caso escolhido 1, faz o wget + crawler
				case 1:
					crawler.run(pedido->host, pedido->path);
					break;
				// caso escolhido 2, termina a execucao do programa
				case 2:
					close(newsockfd);   
					close(iServerfd);
					std::cout<<"\n\t FIM :)";
					exit(-1);
					break;
				// caso escolhido 3, passar info pro navegador
				default:
					if (pedido->port == NULL) pedido->port = (char *) "80";
					try{
						browser_request  = http.converte_Request_to_string(pedido);
						iServerfd = socket.createServerSocket(pedido->host, pedido->port);
						socket.sendToServerSocket(browser_request, iServerfd, total_de_bits_recebidos);
						socket.receiveFromServer(newsockfd, iServerfd);
						std::cout<<"\e[0m\n";
						http.PedidoAnalisado_destroy(pedido);	
						close(newsockfd);   
						close(iServerfd);
					}
					catch(char const* erro){
						std::cout<<"\n"<<erro<<"\n";
					}
					break;
			}
		}
		free(mensagem);
		std::cout<<"\n\e[04;01;31mKeep Using\e[0m\n";     
	}
	close(socket.inSocket);
	close(socket.bind_socket);
	return 0;
}
