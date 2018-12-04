#include "util.hpp"
#include "crawler.hpp"
using namespace std;
queue <string> gquiz;
char *raiz;

Crawler::Crawler() = default;
Crawler::~Crawler() = default;
//  escreve no arquivo a resposta da request
int Crawler::wget (char *host)
{  
    char buffer[65535];
    struct addrinfo host_info;
    struct addrinfo *host_info_list;
    int idSocket;
    FILE *fp1;
    //Obtem informações de endereço para o soquete de fluxo na porta de entrada
    memset(&host_info, 0, sizeof(host_info));
    host_info.ai_family = AF_UNSPEC;
    host_info.ai_socktype = SOCK_STREAM;
    if (getaddrinfo(host, "http", &host_info, &host_info_list) != 0) {
        fprintf(stderr," Nao pode obter a pagina >> ");
        printf("%s\n", host);
        //exit (1);
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
        fprintf(stderr," Erro ao tentar conectar o servidor! O programa foi encerrado\n");
        return 1;
    }
    freeaddrinfo(host_info_list);

    /* "format" is the format of the HTTP request we send to the web
       server. */
    char format[1000];
    memset(format,0,sizeof (format));
    strcpy(format,"GET / HTTP/1.1\r\nHost:");
    strcat(format,host);
    strcat(format,"\r\nConnection: close\r\n\r\n");
    /* Send the request. */
    int bytesSend;
    bytesSend = send (idSocket, format, strlen (format), 0);
    /* Recv data */
    memset(buffer,0,sizeof (buffer));  
    int bytesRecv;
    fp1 = fopen("index.html","w");
    while ((bytesRecv = recv(idSocket, buffer, sizeof (buffer), 0)) > 0) {
        fprintf(fp1,"%s", buffer);         
    } 
    fclose(fp1);
    std::cout<<"endwget";
    return 0;
}
void Crawler::delay (float delay1) {

   if (delay1<0.001) return; // pode ser ajustado e/ou evita-se valores negativos.

   float inst1=0, inst2=0;

   inst1 = (float)clock()/(float)CLOCKS_PER_SEC;

   while (inst2-inst1<delay1) inst2 = (float)clock()/(float)CLOCKS_PER_SEC;

   return;

}
void Crawler::showq(queue <string> gq)
{
    queue <string> g = gq;
    size_t found1, found2, found3, found4, found5, found6;
    while (!g.empty())
    {
        string aux = g.front();
        found1 = aux.find("secom/favicon.ico");
        found2 = aux.find("bootstrap");
        found3 = aux.find("<img src=");
        found4 = aux.find("drupal");
        found5 = aux.find(":");
        found6 = aux.find(".css");
        if(found1==std::string::npos && found2==std::string::npos && found3==std::string::npos && found4==std::string::npos && found5==std::string::npos && found6==std::string::npos && aux.size() > 17)
        {
            printf("\t\t\t");
            printf("-- %s %s-- ", __DATE__,__TIME__);
            cout << g.front() << '\n';
            delay(1);
        }
        g.pop();
    }
    cout << '\n';
}

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


void Crawler::spider(char *host)
{   raiz = host;
    FILE *fp1;
    char dados[5000];
    fp1 = fopen("index.html","r");
    queue <string> auxiliar;
    if(fp1 == NULL)
    {
        printf("Falha ao abrir o arquivo\n");
        cout << host;
        cout << '\n';
        exit(1);
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
    printf("-- %s %s-- ", __DATE__,__TIME__);
    cout << host;cout << " >> ";cout << '\n';
    showq(auxiliar);
    std::cout<<"Fim spider";
}
