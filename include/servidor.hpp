#ifndef SERVIDOR
#define SERVIDOR 
#include <util.hpp>

class Servidor{
private:
    long int porta;
public:
    Servidor(long int porta);
    int init();
    ~Servidor();
};

#endif