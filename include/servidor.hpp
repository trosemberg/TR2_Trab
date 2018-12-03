#ifndef SERVIDOR
#define SERVIDOR 
#include <util.hpp>


class Servidor{
public:
    Servidor(long int porta);
    int init();
    ~Servidor();
private:
    Socket socket;
    HTTP http;
};

#endif