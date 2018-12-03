#ifndef HEADER_H
#define HEADER_H
#include <cstdio>


class HTTP{
public:
	HTTP();
	~HTTP();
	void* getRequestHTTP(void*);
     int PedidoAnalisado_printRequestLine(struct PedidoAnalisado *pr, char * buf, std::size_t buflen, std::size_t *tmp);
     std::size_t PedidoAnalisado_requestLineLen(struct PedidoAnalisado *pr);
     int Analise_do_pedido(struct PedidoAnalisado *p, const char *buffer,int size_buffer);
     void PedidoAnalisado_destroy(struct PedidoAnalisado *p);
     int recuperaPedidoHTTP(struct PedidoAnalisado *p, char *buffer,std::size_t size_buffer);
     int recupera_cabecalho_PedidoHTTP(struct PedidoAnalisado *p, char *buffer, std::size_t size_buffer);
     std::size_t PedidoAnalisado_sizeTotal(struct PedidoAnalisado *p);
     int CabecalhoDoPedido_set(struct PedidoAnalisado *p, const char * key, const char * value);
     struct CabecalhoDoPedido* CabecalhoDoPedido_get(struct PedidoAnalisado *p, const char * key);
     std::size_t CabecalhoDoPedido_size(struct PedidoAnalisado *p);
     int CabecalhoDoPedido_remove (struct PedidoAnalisado *p, const char * key);
     char* converte_Request_to_string(struct PedidoAnalisado *pedido);
	struct PedidoAnalisado* PedidoAnalisado_create();
};

struct PedidoAnalisado {
     char *method; 
     char *protocol; 
     char *host; 
     char *port; 
     char *path;
     char *version;
     char *buf;
     std::size_t buflen;
     struct CabecalhoDoPedido *headers;
     std::size_t headersused;
     std::size_t size_headers;
};

struct CabecalhoDoPedido {
     char * key;
     std::size_t sizeKey;
     char * value;
     std::size_t sizeValue;
};


#endif // HEADER_H