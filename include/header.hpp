#ifndef HEADER_H
#define HEADER_H
#include "util.hpp"
#include "socket.hpp"
#include "crawler.hpp"
#include <cstdio>

// Classe responsavel por fazer as operações necessarias com as requests e reponses em HTTP
class HTTP{
public:
	HTTP(long int porta);
	~HTTP();
	void* RequestHTTP(void*);
     int PedidoAnalisado_printRequestLine(struct PedidoAnalisado *pr, char * buf, std::size_t buflen, std::size_t *tmp);
     std::size_t PedidoAnalisado_requestLineLen(struct PedidoAnalisado *pr);
     int Analise_do_pedido(struct PedidoAnalisado *p, const char *buffer,int size_buffer);
     void PedidoAnalisado_destroy(struct PedidoAnalisado *p);
     int recuperaPedidoHTTP(struct PedidoAnalisado *p, char *buffer,std::size_t size_buffer);
     int recupera_cabecalho_PedidoHTTP(struct PedidoAnalisado *p, char *buffer, std::size_t size_buffer);
     std::size_t PedidoAnalisado_sizeTotal(struct PedidoAnalisado *p);
     int PedidoHeader_set(struct PedidoAnalisado *p, const char * key, const char * value);
     struct PedidoHeader* PedidoHeader_get(struct PedidoAnalisado *p, const char * key);
     int PedidoHeader_remove (struct PedidoAnalisado *p, const char * key);
     char* converte_Request_to_string(struct PedidoAnalisado *pedido);
     void PedidoHeader_create(struct PedidoAnalisado *p);
     int PedidoHeader_printHeaders(struct PedidoAnalisado * pr, char * buf, std::size_t len);
	struct PedidoAnalisado* PedidoAnalisado_create();
     std::size_t PedidoHeader_size(struct PedidoAnalisado *p);
     int Analise_PedidoHeader(struct PedidoAnalisado * pr, char * line);
     std::size_t PedidoHeader_sizeLine(struct PedidoHeader * ph);
     void PedidoHeader_destroyOne(struct PedidoHeader * ph);
     void PedidoHeader_destroy(struct PedidoAnalisado * pr);
private:
     Socket socket;
     Crawler crawler;
};
// struct que define o pedido
struct PedidoAnalisado {
     char *method; 
     char *protocol; 
     char *host; 
     char *port; 
     char *path;
     char *version;
     char *buf;
     std::size_t buflen;
     struct PedidoHeader *headers;
     std::size_t headersused;
     std::size_t size_headers;
};
// struct que associa a chave a um valor na linha de cabecalho do HTTP
struct PedidoHeader {
     char * key;
     std::size_t sizeKey;
     char * value;
     std::size_t sizeValue;
};


#endif // HEADER_H