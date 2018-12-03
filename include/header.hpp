#ifndef HEADER_H
#define HEADER_H

#include <string>
#include <vector>
#include <tuple>

class HTTP{
public:
	HTTP();
	~HTTP();
	void* getRequestHTTP(void*);
	struct PedidoAnalisado *pedido = PedidoAnalisado_create(); 
     int PedidoAnalisado_printRequestLine(struct PedidoAnalisado *pr, char * buf, size_t buflen, size_t *tmp);
     size_t PedidoAnalisado_requestLineLen(struct PedidoAnalisado *pr);
};

struct PedidoAnalisado {
     char *method; 
     char *protocol; 
     char *host; 
     char *port; 
     char *path;
     char *version;
     char *buf;
     size_t buflen;
     struct CabecalhoDoPedido *headers;
     size_t headersused;
     size_t size_headers;
};

struct CabecalhoDoPedido {
     char * key;
     size_t sizeKey;
     char * value;
     size_t sizeValue;
};

struct PedidoAnalisado* PedidoAnalisado_create();

#endif // HEADER_H