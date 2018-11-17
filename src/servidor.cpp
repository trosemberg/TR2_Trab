#include <socket.hpp>
#include <servidor.hpp>
#include <util.hpp>

int Servidor::init(){
    int porta;
    int socket_desc , sock , clientLen , read_size;
    struct sockaddr_in server , client;
    char client_message[200]={0};
    char message[100] = {0};
    const char *pMessage = "hello aticleworld.com";
    porta = this->porta;
    //Create socket
    socket_desc = socket.SocketCreate();
    if (socket_desc == -1){
        printf("Could not create socket");
        return 1;
    }
    printf("Socket created\n");
    //Bind
    if( socket.SocketBind(socket_desc, porta) < 0){
        //print the error message
        perror("bind failed.");
        return 1;
    }
    printf("bind done\n");

    //Listen
    listen(socket_desc , 3);

    //Accept and incoming connection

    while(1){
        printf("Waiting for incoming connections...\n");
        clientLen = sizeof(struct sockaddr_in);
        //accept connection from an incoming client
        sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&clientLen);
        if (sock < 0){
            perror("accept failed");
            return 1;
        }
        printf("Connection accepted\n");
        memset(client_message, '\0', sizeof client_message);
        memset(message, '\0', sizeof message);
        //Receive a reply from the client
        if( recv(sock , client_message , 200 , 0) < 0){
            printf("recv failed");
            break;
        }
        printf("Client reply : %s\n",client_message);
        if(strcmp(pMessage,client_message)==0){
            strcpy(message,"Hi there !");
        }else{
        strcpy(message,"Invalid Message !");
        }
        // Send some data
        if( send(sock , message , strlen(message) , 0) < 0)
        {
            printf("Send failed");
            return 1;
        }
        close(sock);
        sleep(1);
    }
    return 0;
}

Servidor::Servidor( long int porta){
    this->porta = porta;
}
Servidor::~Servidor()= default;