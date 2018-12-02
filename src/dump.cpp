/**
 * @file dump.cpp
 * @brief Código responsável pelo download do código HTML do servidor requisitado
 *
 * Neste módulo, será feito a extração do código HTML do servidor requisitado para uma máquina local, onde será utilizado para recarregar melhor as informação do servidor.
 * Será criado um arquivo .html com o código do site requisitado.
 *
 * @author Raphael Queiroz
 * @author Felipe Brandão
*/
#include "Dump.h"

Dump::Dump() = default;

Dump::~Dump() = default;

/**
 * @fn DumpHTML()
 * @brief Código responsável pela extração do código
 *
 * @param body - body recebido da mensagem HTTP
 * @return
*/
void Dump::DumpHTML( std::string body ){
	unsigned int start = static_cast<unsigned int>(body.find("<"));

	if(start == 0){
		FILE * fp =  fopen ( "dump/dump.html", "w+" );
		if( fp == nullptr) printf("\nArquivo não existe\n");
		else {
			fprintf( fp, body.c_str() );
			fclose( fp );
			printf("\nDump criado\n");
		}
	}
}