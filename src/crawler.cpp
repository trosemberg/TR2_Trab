#include "util.hpp"
#include "crawler.hpp"
using namespace std;
queue <string> gquiz;
char *raiz;

Crawler::Crawler() = default;
Crawler::~Crawler() = default;
//  escreve no arquivo a resposta da request
int Crawler::wget (char *host, char *path){

    char buffer[65535];
    struct addrinfo host_info;
    struct addrinfo *host_info_list;
    int idSocket;
    std::string caminho(path);
    std::size_t found = caminho.find_first_of("/");
    // std::cout<<"ENTER WGET";
    while (found!=std::string::npos){
        caminho[found]='-';
        found=caminho.find_first_of("/",found+1);
    }

    FILE *fp1;
    //Obtem informações de endereço para o soquete de fluxo na porta de entrada
    memset(&host_info, 0, sizeof(host_info));
    host_info.ai_family = AF_UNSPEC;
    host_info.ai_socktype = SOCK_STREAM;
    if (getaddrinfo(host, "http", &host_info, &host_info_list) != 0) {
        return 1;
    }
    //cria um socket
    if ((idSocket = socket(host_info_list->ai_family, host_info_list->ai_socktype, host_info_list->ai_protocol)) < 0) 
    {
        fprintf(stderr," Erro ao criar socket para o servidor! O programa foi encerrado\n");
        return 1;
    }
    //faz a conecção
    if (connect(idSocket, host_info_list->ai_addr, host_info_list->ai_addrlen) < 0)
    {
        return 1;
    }
    freeaddrinfo(host_info_list);

    /* "format" is the format of the HTTP request we send to the web
       server. */
    char format[2000];
    memset(format,0,sizeof (format));
    strcpy(format,"GET http://");
    strcat(format,host);
    strcat(format,path);
    strcat(format," HTTP/1.1\r\nHost:");
    strcat(format,host);
    // strcat(format,"\r\nAccept: text/html");
    strcat(format,"\r\nConnection: close\r\n\r\n");
    // std::cout<<format;
    /* Send the request. */
    int bytesSend;
    bytesSend = send (idSocket, format, strlen (format), 0);
    /* Recv data */
    memset(buffer,0,sizeof (buffer));  
    int bytesRecv;
    char arquivo[1000] = "./html/";
    strcat(arquivo,host);
    strcat(arquivo,caminho.c_str());
    strcat(arquivo,".html");
    fp1 = fopen(arquivo,"w");
    while ((bytesRecv = recv(idSocket, buffer, sizeof (buffer), 0)) > 0) {
        fprintf(fp1,"%s", buffer);         
    } 
    fclose(fp1);
    // std::cout<<"EXIT WGET";
    return 0;
}
// compara pra ver se ja esta na fila a string passada
int Crawler::compara(queue <string> gq, string name)
{
    queue <string> g = gq;
    while (!g.empty())
    {
        string str = g.front();
        if(str.compare(name) != 0) g.pop();
        else return 1; //retorna 1 se for igual
    }
    return 0;
}

//retorna uma fila com os links achados
std::queue <std::string> Crawler::spider(char *host,char *path)
{   raiz = host;
    FILE *fp1;
    char dados[5000];
    char arquivo[1000]="./html/";
    std::string caminho(path);
    // std::cout<<"ENTER SPIDER";
    std::size_t found = caminho.find_first_of("/");
    while (found!=std::string::npos){
        caminho[found]='-';
        found=caminho.find_first_of("/",found+1);
    }
    strcat(arquivo,host);
    strcat(arquivo,caminho.c_str());
    strcat(arquivo,".html");
    fp1 = fopen(arquivo,"r");
    queue <string> auxiliar;
    if(fp1 == NULL)
    {
        printf("\nFalha ao abrir o arquivo\n");
        cout << host;
        cout << '\n';
        return auxiliar;
    }else{
        string domain = raiz;
        size_t pos = domain.find("www.");
        domain = domain.substr(pos+4);
        while(fgets(dados,5000, fp1) != NULL) //le o arquivo de entrada
        {
            string line = dados;
            size_t pos = line.find("href=");
            if (pos!=std::string::npos)
            {
                string hostname;
                string str1 = line.substr(pos+5);
                pos = str1.find(" ");
                string url = str1.substr(0,pos);
                size_t pos1 = url.find_first_of("http://");
                size_t pos2 = url.find_last_of('"');
                if (pos1!=std::string::npos && pos2!=std::string::npos)
                {
                    url = url.substr(pos1, pos2-1);
                    pos = url.find("http://");
                    if (pos!=std::string::npos)
                    {
                        hostname = url.substr(pos+7);
                        pos = hostname.find_last_of('/');
                        if(pos!=std::string::npos) hostname = hostname.substr(0,pos);
                        pos = hostname.find(domain);
                        if (pos!=std::string::npos && hostname.compare(host) != 0 && compara(gquiz,hostname) != 1)
                        {
                            gquiz.push(hostname);
                            auxiliar.push(hostname);
                        }
                    }else{
                        pos = url.find("https://");
                        if (pos==std::string::npos)
                        {
                            hostname = raiz + url;
                            pos = hostname.find_last_of('/');
                            if(pos!=std::string::npos) hostname = hostname.substr(0,pos);
                            if (hostname.compare(host) != 0 && compara(gquiz,hostname) != 1)
                            {
                                gquiz.push(hostname);
                                auxiliar.push(hostname);
                            }
                        }else{
                            hostname = url.substr(pos+8);
                            pos = hostname.find_last_of('/');
                            if(pos!=std::string::npos) hostname = hostname.substr(0,pos);
                            pos = hostname.find(domain);
                            if (pos!=std::string::npos && hostname.compare(host) != 0 && compara(gquiz,hostname) != 1)
                            {
                                gquiz.push(hostname);
                                auxiliar.push(hostname);
                            }
                        }
                    }
                }
                pos = str1.find("<img src=");
                if (pos!=std::string::npos)
                {
                    string img = str1.substr(pos+9);
                    pos = img.find(" ");
                    if (pos!=std::string::npos) img = img.substr(0,pos);
                    size_t pos1 = img.find_first_of('"');
                    size_t pos2 = img.find_last_of('"');
                    if (pos1!=std::string::npos && pos2!=std::string::npos)
                    {
                        string Img = img.substr(pos1+2, pos2+1);
                        pos = Img.find('"');
                        if (pos!=std::string::npos) Img = Img.substr(0,pos);
                        string link = hostname + "/" + Img;
                        if(compara(gquiz,link) != 1) {
                            gquiz.push(link);
                            auxiliar.push(hostname);
                        }
                    }
                }
            }
            pos = line.find("<li class=");
            size_t pos5 = line.find("<ul class=");
            if (pos!=std::string::npos || pos5!=std::string::npos)
            {
                size_t pos = line.find("href=");
                if (pos!=std::string::npos)
                {
                    string hostname;
                    string str1 = line.substr(pos+5);
                    pos = str1.find(" ");
                    string url = str1.substr(0,pos);
                    size_t pos1 = url.find_first_of("http://");
                    size_t pos2 = url.find_last_of('"');
                    if (pos1!=std::string::npos && pos2!=std::string::npos)
                    {
                        url = url.substr(pos1, pos2-1);
                        pos = url.find("http://");
                        if (pos!=std::string::npos)
                        {
                            hostname = url.substr(pos+7);
                            pos = hostname.find_last_of('/');
                            if(pos!=std::string::npos) hostname = hostname.substr(0,pos);
                            pos = hostname.find(domain);
                            if (pos!=std::string::npos && hostname.compare(host) != 0 && compara(gquiz,hostname) != 1)
                            {
                                gquiz.push(hostname);
                                auxiliar.push(hostname);
                            }
                        }else{
                            pos = url.find("https://");
                            if (pos==std::string::npos)
                            {
                                hostname = raiz + url;
                                pos = hostname.find_last_of('/');
                                if(pos!=std::string::npos) hostname = hostname.substr(0,pos);
                                pos = hostname.find(domain);
                                if (pos!=std::string::npos && hostname.compare(host) != 0 && compara(gquiz,hostname) != 1)
                                {
                                    gquiz.push(hostname);
                                    auxiliar.push(hostname);
                                }
                            }else{
                                hostname = url.substr(pos+8);
                                pos = hostname.find_last_of('/');
                                if(pos!=std::string::npos) hostname = hostname.substr(0,pos);
                                pos = hostname.find(domain);
                                if (pos!=std::string::npos && hostname.compare(host) != 0 && compara(gquiz,hostname) != 1)
                                {
                                    gquiz.push(hostname);
                                    auxiliar.push(hostname);
                                }
                            }
                        }
                    }
                    pos = str1.find("<img src=");
                    if (pos!=std::string::npos)
                    {
                        string img = str1.substr(pos+9);
                        pos = img.find(" ");
                        if (pos!=std::string::npos) img = img.substr(0,pos);
                        size_t pos1 = img.find_first_of('"');
                        size_t pos2 = img.find_last_of('"');
                        if (pos1!=std::string::npos && pos2!=std::string::npos)
                        {
                            string Img = img.substr(pos1+2, pos2+1);
                            pos = Img.find('"');
                            if (pos!=std::string::npos) Img = Img.substr(0,pos);
                            string link = hostname + "/" + Img;
                            if(compara(gquiz,link) != 1) {
                                gquiz.push(link);
                                auxiliar.push(hostname);
                            }
                        }
                    }
                }
            }
        }
    }
    fclose(fp1);
    return auxiliar;
}
//roda o spider e o wget com profundidade 2
void Crawler::run(char *host,char *path){
    queue <string> first,second,third;
    int wget_ret;
    std::cout<<host<<path<<"\n\e[95m";
    char hoost[100] = "/", paath[100] = "/", temp[100],*temp_2;
    wget_ret = wget(host,path);
    if (wget_ret == 0){
        first = spider(host,path);
        while(!first.empty()){
            std::strcpy(hoost,"/");
            std::strcpy(paath,"/");
            std::cout<<"\t-"<<first.front()<<"\n";
            strcpy(temp,(char *)first.front().c_str());
            temp_2 = strtok (temp,"/");
            std::strcpy(hoost,temp_2);
            temp_2 = strtok (NULL,"/");
            if (temp_2 == NULL) {          
                std::strcpy(paath,"/");
            }else{
                std::strcat(paath,temp_2);
            }
            wget_ret = wget(hoost,paath);
            first.pop();
            if(wget_ret == 0){
                second = spider(hoost,paath);
                while(!second.empty()){
                    std::strcpy(hoost,"/");
                    std::strcpy(paath,"/");
                    std::cout<<"\t-\t-"<<second.front()<<"\n";
                    strcpy(temp,(char *)second.front().c_str());
                    temp_2 = strtok (temp,"/");
                    std::strcpy(hoost,temp_2);
                    temp_2 = strtok (NULL,"/");
                    if (temp_2 == NULL) {         
                        std::strcpy(paath,"/");
                    }else{
                        std::strcat(paath,temp_2);
                    }
                    wget_ret = wget(hoost,paath);
                    second.pop();
                    if(wget_ret == 0){
                        third = spider(hoost,paath);   
                        while(!third.empty()){
                            std::cout<<"\t-\t-\t"<<third.front()<<"\n";
                            third.pop();
                        }
                    }
                }
            }
        }
    }
    std::cout<<"\e[0m\n\nFim spider + wget recursivo de profundidade 2\n\n";
}