#ifndef HEADER_H
#define HEADER_H



class HTTP{
public:
	HTTP();
	~HTTP();
	void* getRequestHTTP(void*);
     int PedidoAnalisado_printRequestLine(struct PedidoAnalisado *pr, char * buf, size_t buflen, size_t *tmp);
     size_t PedidoAnalisado_requestLineLen(struct PedidoAnalisado *pr);
     int Analise_do_pedido(struct PedidoAnalisado *p, const char *buffer,int size_buffer);
     void PedidoAnalisado_destroy(struct PedidoAnalisado *p);
     int recuperaPedidoHTTP(struct PedidoAnalisado *p, char *buffer, size_t size_buffer);
     int recupera_cabecalho_PedidoHTTP(struct PedidoAnalisado *p, char *buffer, size_t size_buffer);
     size_t PedidoAnalisado_sizeTotal(struct PedidoAnalisado *p);
     int CabecalhoDoPedido_set(struct PedidoAnalisado *p, const char * key, const char * value);
     struct CabecalhoDoPedido* CabecalhoDoPedido_get(struct PedidoAnalisado *p, const char * key);
     size_t CabecalhoDoPedido_size(struct PedidoAnalisado *p);
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


#endif // HEADER_H