// stringstreams
#include <iostream>
#include <string>
#include <stdlib.h>
#include <servidor.hpp>

#define PORTA 8228

int main(int argc, char const *argv[]){
  long int porta = 0;
  std::string flag,porta_escolhida;
  
  switch (argc){
    case 1:
    porta = PORTA;
      break;
    case 3:
      flag = argv[1];
      porta = strtol(argv[2], nullptr, 10);

      if (porta < 1024 || porta > 65535) {
        std::cout<<"\nO numero da porta deve estar no range valido de 1024 a 65535\n";
        exit(-1);
      }else if(flag.compare("-p") !=0){
        std::cout<< "a flag deve ser -p\n";
        exit(-1);
      }
      break;
    default:
      std::cout<< "O formato do programa deve ser:\n \"./aracne\" ou "<<std::endl;
      std::cout<< "\"./aracne -p (numero da porta)\"\n";
      exit(-1);
      break;
  }
  std::cout<<"\n\nConectando a porta:"<< porta<<"...";
  Servidor server((long int)porta);
  server.init();
  return 0;
}
