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
		printf("----------------------------------------------------\n");
		printf("Resquisicao HTTP do browser:\n");
		printf("%s\n", mensagem);
		printf("1 - Spider \n2-Apenas responder o browser\nDigite a opcao >> ");
		scanf("%d", &opcao);
		switch(opcao)
		{
			case 1:  
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

char* HTTP::converte_Request_to_string(struct PedidoAnalisado *pedido)
{
	char *serverRequest, *cabecalhoBuffer;
	int sizeRequest, sizeCabecalho;
	/* Seta o cabecalho */
	CabecalhoDoPedido_set(pedido, "Host", pedido->host);
	CabecalhoDoPedido_set(pedido, "Connection", "close");

	sizeCabecalho = CabecalhoDoPedido_size(pedido);
	cabecalhoBuffer = (char*) malloc(sizeCabecalho + 1);

	if (cabecalhoBuffer == NULL) {
		fprintf(stderr," Erro na alocacao do cabecalhoBuffer! \n");
		exit (1);
	}

	recupera_cabecalho_PedidoHTTP(pedido, cabecalhoBuffer, sizeCabecalho);
	cabecalhoBuffer[sizeCabecalho] = '\0';

	sizeRequest = strlen(pedido->method) + strlen(pedido->path) + strlen(pedido->version) + sizeCabecalho + 4;
	serverRequest = (char *) malloc(sizeRequest + 1);

	if(serverRequest == NULL){
		fprintf(stderr," Erro na alocacao do serverRequest!\n");
		exit (1);
	}
	serverRequest[0] = '\0';
	strcpy(serverRequest, pedido->method);
	strcat(serverRequest, " ");
	strcat(serverRequest, pedido->path);
	strcat(serverRequest, " ");
	strcat(serverRequest, pedido->version);
	strcat(serverRequest, "\r\n");
	strcat(serverRequest, cabecalhoBuffer);
	free(cabecalhoBuffer);
	return serverRequest;
}


int HTTP::Analise_do_pedido(struct PedidoAnalisado *p, const char *buffer, int size_buffer)
{
     char *full_addr;
     char *saveptr;
     char *index;
     char *currentHeader;

     if (p->buf != NULL) {
	  std::cout<<"pobjeto de análise já atribuído a uma solicitação\n";
	  return -1;
     }
   
     if (size_buffer < MIN_REQ_LEN || size_buffer > MAX_REQ_LEN) {
	  std::cout<<"tamanho do buffer e invalido %d", size_buffer;
	  return -1;
     }
   
     /* Create NUL terminated tmp buffer */
     char *tmp_buf = (char *)malloc(size_buffer + 1); /* including NUL */
     memcpy(tmp_buf, buffer, size_buffer);
     tmp_buf[size_buffer] = '\0';
   
     index = strstr(tmp_buf, "\r\n\r\n");
     if (index == NULL) {
	  std::cout<<"linha de solicitacao invalida, sem fim de cabecalho\n";
	  free(tmp_buf);
	  return -1;
     }
   
     /* Copy request line into parse->buf */
     index = strstr(tmp_buf, "\r\n");
     if (p->buf == NULL) {
	  p->buf = (char *) malloc((index-tmp_buf)+1);
	  p->buflen = (index-tmp_buf)+1;
     }
     memcpy(p->buf, tmp_buf, index-tmp_buf);
     p->buf[index-tmp_buf] = '\0';

     /* Parse request line */
     p->method = strtok_r(p->buf, " ", &saveptr);
     if (p->method == NULL) {
	  std::cout<< "linha de solicitacao invalida, sem espaco em branco\n";
	  free(tmp_buf);
	  free(p->buf);
	  p->buf = NULL;
	  return -1;
     }

     full_addr = strtok_r(NULL, " ", &saveptr);

     if (full_addr == NULL) {
	  std::cout<< "linha de solicitacao invalida, nao possui o endereco completo\n";
	  free(tmp_buf);
	  free(p->buf);
	  p->buf = NULL;
	  return -1;
     }

     p->version = full_addr + strlen(full_addr) + 1;

     if (p->version == NULL) {
	  std::cout<< "linha de solicitacao invalida, falta a versao do http\n";
	  free(tmp_buf);
	  free(p->buf);
	  p->buf = NULL;
	  return -1;
     }
     if (strncmp (p->version, "HTTP/", 5)) {
	  std::cout<< "linha de solicitacao invalida, versao nao suportada %s\n", p->version;
	  free(tmp_buf);
	  free(p->buf);
	  p->buf = NULL;
	  return -1;
     }
     p->protocol = strtok_r(full_addr, "://", &saveptr);
     if (p->protocol == NULL) {
	  std::cout<< "linha de solicitacao invalida, falta a porta\n";
	  free(tmp_buf);
	  free(p->buf);
	  p->buf = NULL;
	  return -1;
     }
     
     const char *rem = full_addr + strlen(p->protocol) + strlen("://");
     size_t abs_uri_len = strlen(rem);

     p->host = strtok_r(NULL, "/", &saveptr);
     if (p->host == NULL) {
	  std::cout<< "linha de solicitacao invalida, nao possui host:\n";
	  free(tmp_buf);
	  free(p->buf);
	  p->buf = NULL;
	  return -1;
     }
     
     if (strlen(p->host) == abs_uri_len) {
	  std::cout<<"linha de solicitação inválida, sem caminho absoluto\n";
	  free(tmp_buf);
	  free(p->buf);
	  p->buf = NULL;
	  return -1;
     }
     p->path = strtok_r(NULL, " ", &saveptr);
     if (p->path == NULL) {          // replace empty abs_path with "/"
	  int rlen = strlen(root_abs_path);
	  p->path = (char *)malloc(rlen + 1);
	  strncpy(p->path, root_abs_path, rlen + 1);
     } else if (strncmp(p->path, root_abs_path, strlen(root_abs_path)) == 0) {
	  std::cout<<"linha de solicitação inválida, o caminho não pode começar "" com dois caracteres de barra\n";
	  free(tmp_buf);
	  free(p->buf);
	  p->buf = NULL;
	  p->path = NULL;
	  return -1;
     } else {
	  // copia p->path, prefixo com uma barra
	  char *tmp_path = p->path;
	  int rlen = strlen(root_abs_path);
	  int plen = strlen(p->path);
	  p->path = (char *)malloc(rlen + plen + 1);
	  strncpy(p->path, root_abs_path, rlen);
	  strncpy(p->path + rlen, tmp_path, plen + 1);
     }

     p->host = strtok_r(p->host, ":", &saveptr);
     p->port = strtok_r(NULL, "/", &saveptr);

     if (p->host == NULL) {
	  std::cout<< "linha de solicitacao invalida, nao possui host:\n";
	  free(tmp_buf);
	  free(p->buf);
	  free(p->path);
	  p->buf = NULL;
	  p->path = NULL;
	  return -1;
     }

     if (p->port != NULL) {
	  int port = strtol (p->port, (char **)NULL, 10);
	  if (port == 0 && errno == EINVAL) {
	       std::cout<<"linha de solicitacao invalida, porta incorreta: %s\n", p->port;
	       free(tmp_buf);
	       free(p->buf);
	       free(p->path);
	       p->buf = NULL;
	       p->path = NULL;
	       return -1;
	  }
     }

   
     /* Parse headers */
     int ret = 0;
     currentHeader = strstr(tmp_buf, "\r\n")+2;
     while (currentHeader[0] != '\0' && 
	    !(currentHeader[0] == '\r' && currentHeader[1] == '\n')) {

	  if (Analise_CabecalhoDoPedido(p, currentHeader)) {
	       ret = -1;
	       break;
	  }

	  currentHeader = strstr(currentHeader, "\r\n");
	  if (currentHeader == NULL || strlen (currentHeader) < 2)
	       break;

	  currentHeader += 2;
     }
     free(tmp_buf);
     return ret;
}

size_t HTTP::PedidoAnalisado_requestLineLen(struct PedidoAnalisado *pr)
{
     if (!pr || !pr->buf)
	  return 0;

     size_t len =  
	  strlen(pr->method) + 1 + strlen(pr->protocol) + 3 + 
	  strlen(pr->host) + 1 + strlen(pr->version) + 2;
     if(pr->port != NULL)
     {
	  len += strlen(pr->port)+1;
     }
     /*caminho é pelo menos uma barra */
     len += strlen(pr->path);
     return len;
}

struct PedidoAnalisado* PedidoAnalisado_create()
{
     struct PedidoAnalisado *pr;
     pr = (struct PedidoAnalisado *)malloc(sizeof(struct PedidoAnalisado));
     if (pr != NULL)
     {
	  CabecalhoDoPedido_create(pr);
	  pr->buf = NULL;
	  pr->method = NULL;
	  pr->protocol = NULL;
	  pr->host = NULL;
	  pr->path = NULL;
	  pr->version = NULL;
	  pr->buf = NULL;
	  pr->buflen = 0;
     }
     return pr;
}