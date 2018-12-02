#include <socket.hpp>
#include <servidor.hpp>
#include <util.hpp>
#include <header.hpp>

int Servidor::init(){
    //Create socket
    socket.bind_socket = socket.SocketCreate();
    if (socket.bind_socket == -1){
        printf("Could not create socket");
        return 1;
    }
    printf("Socket created\n");
    //Bind
    socket.SocketBind();
    if( socket.bind_socket < 0){
        //print the error message
        perror("bind failed.");
        return 1;
    }

    printf("bind done \n");    
    printf("Sucessfully conected with server\n\n");
    while(1){
        std::string message;
        //Connect to remote server
        socket.SocketAccept();
        socket.GetRequest();

        if(!requestsRcv.empty())
			std::swap( requestsRcv, toSendRqst );

		if(!toSendRqst.empty()) {
			printf("\nEnviando %d request(s)...\n", (int) toSendRqst.size());

			for (auto &i : toSendRqst) {
				printf("\nEnviando request para %s:%s\n", i.host.c_str(), i.port.c_str());
				printf("\n%s\n", i.to_string().c_str());
				if(socket.SendOut(i) == -1){
					printf("\nNão foi possível enivar request\n");
					exit(1);
				}
			}
			
			toSendRqst.clear();
		}
		
		socket.AnswerRequest();

		if(!responsesRcv.empty())
			std::swap(responsesRcv, toSendRsp);

		if(!toSendRsp.empty()) {
			printf("\nEnviando %d response(s)...\n", (int) toSendRsp.size());

			for(auto &i : toSendRsp) {
				printf("\nResponse: \n%s\n", i.to_string().c_str());
				if(socket.SendIn(i) == -1){
					printf("\nNão foi possível enivar response\n");
					exit(1);
				}
			}

			toSendRsp.clear();
		}
    }
}

Servidor::Servidor(long int porta) :  requestsRcv(socket.requestsRcv),
							        responsesRcv(socket.responsesRcv),
                                    socket(porta) {}
                            
Servidor::~Servidor()= default;