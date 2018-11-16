#include <./../include/servidor.hpp>
#include <iostream>

void Servidor::init(){
    std::cout<< this->porta;
}

Servidor::Servidor( long int porta){
    this->porta = porta;
}
Servidor::~Servidor()= default;