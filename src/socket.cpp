#include <socket.hpp>
#include <servidor.hpp>
#include <util.hpp>
Socket::Socket() = default;
Socket::~Socket() = default;

//Create a Socket for server communication
short Socket::SocketCreate()
{ 
    short hSocket;
    printf("Create the socket\n");
    hSocket = socket(AF_INET, SOCK_STREAM, 0);
    return hSocket;
}

//try to connect with server
int Socket::SocketConnect(int hSocket, int porta)
{

    int iRetval=-1;
    struct sockaddr_in remote={0};

    remote.sin_addr.s_addr = inet_addr("127.0.0.1"); //Local Host
    remote.sin_family = AF_INET;
    remote.sin_port = htons(porta);

    iRetval = connect(hSocket , (struct sockaddr *)&remote , sizeof(struct sockaddr_in));


    return iRetval;
}


// Send the data to the server and set the timeout of 20 seconds
int Socket::SocketSend(int hSocket,char* Rqst,short lenRqst)

{

    int shortRetval = -1;
    struct timeval tv;
    tv.tv_sec = 20;  /* 20 Secs Timeout */
    tv.tv_usec = 0;  

    if(setsockopt(hSocket, SOL_SOCKET, SO_SNDTIMEO, (char *)&tv,sizeof(tv)) < 0)
    {
        printf("Time Out\n");
        return -1;
    }
    shortRetval = send(hSocket , Rqst , lenRqst , 0);

    return shortRetval;
}