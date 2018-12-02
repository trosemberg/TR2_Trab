#ifndef SERVIDOR
#define SERVIDOR 
#include <util.hpp>


class Servidor{
public:
    Servidor(long int porta);
    int init();
    ~Servidor();
    std::vector< HTTP::Header > &requestsRcv;
	std::vector< HTTP::Header > toSendRqst;
	std::vector< HTTP::Header > &responsesRcv;
	std::vector< HTTP::Header > toSendRsp;
private:
    Socket socket;
};

#endif