#include "header.hpp"
#include <util.hpp>

HTTP::~HTTP()= default;
HTTP::HTTP() = default;

void* HTTP::getRequestHTTP(void* socketid)
{
	int sizeBuffer = 5000;
	int opcao;
	char buffer[sizeBuffer];

	int newsockfd = *((int*)socketid);

	int total_de_bits_recebidos = 0, recvd;

	int iServerfd;

	char *mensagem;  // armazena a mensagem da URL

	char *browser_request;

	mensagem = (char *) malloc(sizeBuffer); 

	mensagem[0] = '\0';

	while (strstr(mensagem, "\r\n\r\n") == NULL) {  // determines end of request

	  recvd = recv(newsockfd, buffer, sizeBuffer, 0) ;

	  if(recvd < 0 ){
	  	fprintf(stderr," Erro ao receber mensagem do cliente!\n");
		exit (1);
	  				
	  }else if(recvd == 0) {
	  		break;
	  } else {
	  	total_de_bits_recebidos += recvd;
	  	//Se o taman ho da mensagem for maior que o tamanho da string buffer, dobra o tamanho da string
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
	if(strlen(mensagem) > 0)
	{
		char what[5000];
		struct PedidoAnalisado *pedido;    // contem o pedido analisado
		printf("----------------------------------------------------\n");
		printf("Resquisicao HTTP do browser:\n");
		printf("%s\n", mensagem);
		printf("1 - Spider \n2-Apenas responder o browser\nDigite a opcao >> ");
		scanf("%d", &opcao);
		switch(opcao)
		{
			case 1:  
				pedido = PedidoAnalisado_create();
				Analise_do_pedido(pedido, mensagem, strlen(mensagem));
			    memset(what,'\0',5000);
			    strcpy(what,"./spider ");
			    strcat(what,pedido->host);
			    system(what);
				break;
			default:

				pedido = PedidoAnalisado_create();

				Analise_do_pedido(pedido, mensagem, strlen(mensagem));
				//Se a porta não foi setada na mensagem URL, coloquei como padrao a porta 80
				if (pedido->port == NULL) pedido->port = (char *) "80";
				
				int pid = fork();

		 		if(pid == 0)	//processo filho
		 		{
		 			browser_request  = converte_Request_to_string(pedido);		
					iServerfd = createServerSocket(pedido->host, pedido->port);
					sendToServerSocket(browser_request, iServerfd, total_de_bits_recebidos);
					receiveFromServer(newsockfd, iServerfd);
					PedidoAnalisado_destroy(pedido);	
					close(newsockfd);   
					close(iServerfd);
					_exit(0);
		 		}
				break;
		}
	}
	int y = 3;
	int *p = &y;
	return p;
}