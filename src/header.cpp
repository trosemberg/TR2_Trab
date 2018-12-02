/**
 * @file header.cpp
 * @brief Código responsável pela formatação da mensagem no formato HTTP
 *
 * Neste módulo, será feito a formatação da mensagem de resposta e requisição no formato HTTP de acordo com o estudo em sala de aula.
 *
 * @author Raphael Queiroz
 * @author Felipe Brandão
*/
#include "Header.h"

namespace HTTP {
/**
 * @fn Header()
 * @brief Código responsável pela formatação no formato HTTP
 *
 * @param msg - mensagem interceptado pelo socket
 * @return
*/
Header::Header(std::string& msg) {
	if(!msg.empty()){
		unsigned int start = 0;
        auto end = static_cast<unsigned int>(msg.find("\r\n"));
		startLine = msg.substr(start, end);

		start = end + 2;

		while((end = static_cast<unsigned int>(msg.find("\r\n", start)), end) > start){
			std::string linha = msg.substr(start, end-start);
			std::string hostName = linha.substr(0, (linha.find(':')));
			std::string valor = linha.substr((linha.find(':')) + 2);

			if(hostName == "Host"){
				host = valor.substr(0, (valor.find(':')));
				if(valor.size() > (valor.find(':')))
					port = valor.substr((valor.find(':')) + 2);
			}

			fields.push_back(std::make_tuple(hostName, valor));
			
			start = end + 2;
		}

		if(host.empty()){
            auto init = static_cast<unsigned int>(startLine.find("//") + 2);
            auto fim = static_cast<unsigned int>(startLine.find("//", init));
			std::string valor = startLine.substr(init, fim - init);
			host = valor.substr(0, (valor.rfind(':')));
			if(valor.size() > (valor.rfind(':')))
				port = valor.substr((valor.rfind(':')) + 2);
		}

		if(host[0] == '[')
			host = host.substr(1, host.size() - 2);

		if(port.empty())
			port = "80";

		body = msg.substr(start + 2);

		if(!body.empty()){
			Dump::DumpHTML( body );
		}
	}
}

/**
 * @fn to_string()
 * @brief Código responsável por transformar a mensagem em string para melhor leitura
 *
 * @param include - booleano para verificar se deseja incluir o body da mensagem ou não
 * @return
*/
std::string Header::to_string(bool include){
	std::string msg;

	msg += startLine + "\r\n";
	for (auto &i : fields)
        msg += std::get<0>(i) + ": " + std::get<1>(i) + "\r\n";
	msg +="\r\n";
	msg += include? body : "";

	return msg;
}

}