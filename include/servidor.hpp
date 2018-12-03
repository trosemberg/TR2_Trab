#ifndef SERVIDOR
#define SERVIDOR 
#include <util.hpp>
#include <header.hpp>

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