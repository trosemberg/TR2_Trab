#ifndef SERVIDOR
#define SERVIDOR 
#include "util.hpp"
#include "header.hpp"

// classe Servidor, responsavel por rodar o programa
// baseando se na classe socket e http

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