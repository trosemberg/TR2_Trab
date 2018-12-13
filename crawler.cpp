#include "util.hpp"
#include "crawler.hpp"
using namespace std;
char *raiz;

Crawler::Crawler() = default;
Crawler::~Crawler() = default;
//  escreve no arquivo a resposta da request
int Crawler::wget (char *host, char *path, int atual, int max){
    std::fstream file;
    char buffer[65535];
    struct addrinfo host_info;
    struct addrinfo *host_info_list;
    int idSocket;
    // std::size_t achou;
    std::string moved = "";
    std::string URL(host);
    URL  = URL + path;
    for (std::vector<std::string>::iterator it = acessados.begin() ; it != acessados.end(); ++it){
        if(URL.compare(*it) == 0){
            return 0;
        }
    }
    if(atual>=max){
        return 0;
    }
    std::string caminho(path);
    std::size_t found = caminho.find_first_of("/");
    // std::cout<<"ENTER WGET";
    while (found!=std::string::npos){
        caminho[found]='_';
        found=caminho.find_first_of("/",found);
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
    acessados.push_back(URL);
    fclose(fp1);
    file.open(arquivo,std::ios::in);
    std::string msg;
    msg.assign((std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>());
    file.close();
    spider(host,path,atual,max);
    return 0;
}

//retorna uma fila com os links achados
std::queue <std::string> Crawler::spider(char *host,char *path, int atual, int max){
    raiz = host;
    FILE *fp1;
    char hoost[100] = "/", paath[100] = "/", temp[100],*temp_2;
    char dados[5000];
    std::fstream file;
    std::string HTML = ".html";
    char arquivo[1000]="./html/";
    std::string URL = "";
    std::string caminho(path);
    std::size_t achou;
    std::size_t found,found_2;
    std::size_t achado = caminho.find_first_of("/");
    std::string str2;
    while (achado!=std::string::npos){
        caminho[achado]='_';
        achado=caminho.find_first_of("/",achado);
    }
    strcat(arquivo,host);
    strcat(arquivo,caminho.c_str());
    strcat(arquivo,".html");
    fp1 = fopen(arquivo,"r");
    queue <string> auxiliar;
    queue <string> imagens;
    if(fp1 == NULL)
    {
        printf("\nFalha ao abrir o arquivo\n");
        cout << host;
        cout << '\n';
        return auxiliar;
    }else{
        while(fgets(dados,5000, fp1) != NULL) //le o arquivo de entrada
        {
            string line = dados;
            for (std::string hlink : ExtractHyperlinks(line)){
                auxiliar.push(hlink);
            }
            for (std::string hlink : ExtractImageslinks(line)){
                imagens.push(hlink);
            }
            for (std::string hlink : ExtractCssLinks_1(line)){
                imagens.push(hlink);
            }
            for (std::string hlink : ExtractCssLinks_2(line)){
                imagens.push(hlink);
            }
        }
    }
    fclose(fp1);
    while(!imagens.empty()){
        RequestImage(imagens.front(),host);
        imagens.pop();
    }
    file.open(arquivo,std::ios::in);
    std::string msg;
    msg.assign((std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>());
    file.close();
    if(!msg.empty()){
        achou = msg.find("<");
        if(achou!=std::string::npos){
            std::string result = msg.substr(achou);
            file.open(arquivo,std::ios::out);
            file << result;
            file.close();
        }
    }
    file.open(arquivo,std::ios::in);
    msg.assign((std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>());
    file.close();
    if(!msg.empty()){
        found = msg.find("<a href=\"http://");
        found_2 = msg.find("\"",found+10);
        while(found!=std::string::npos){
            if(found!=std::string::npos && found_2!=std::string::npos){
                msg.erase(found+9,7);
                found_2 = msg.find("\"",found+10);
                if(found_2!=std::string::npos){
                    str2 = msg.substr (found,found_2-found);
                    std::replace( str2.begin(), str2.end(), '/', '_');
                    str2+=HTML;
                    msg.replace(found,found_2-found,str2);
                }
            }
            found = msg.find("<a href=\"http://",found+1);
        }
    }
    file.open(arquivo,std::ios::out);
    file << msg;
    file.close();

    while(!auxiliar.empty()){
        std::strcpy(hoost,"/");
        std::strcpy(paath,"/");
        for(int i =0;i<atual;i++){
            std::cout<<"\t-";
        }
        std::cout<<auxiliar.front();
        for (std::vector<std::string>::iterator it = acessados.begin() ; it != acessados.end(); ++it){
            if(auxiliar.front().compare(*it) == 0){
                std::cout<< " \e[96mX\e[95m";
            }
        }
        std::cout<<"\n";
        strcpy(temp,(char *)auxiliar.front().c_str());
        temp_2 = strtok (temp,"/");
        std::strcpy(hoost,temp_2);
        temp_2 = strtok (NULL,"/");
        if (temp_2 == NULL) {
            std::strcpy(paath,"/");
        }else{
            std::strcat(paath,temp_2);
            std::strcat(paath,"/");
        }
        URL += hoost;
        URL += paath;
        wget(hoost,paath,atual + 1,max);
        auxiliar.pop();
    }
    return auxiliar;
}

void Crawler::RequestImage(std::string imagem,char* host){
    std::ofstream file;
    char buffer[65535];
    unsigned char buff;
    vector <unsigned char> response;
    std::size_t found,end_of_file;
    struct addrinfo host_info;
    struct addrinfo *host_info_list;
    int idSocket;
    std::string comando="mkdir -p ./html/";
    if (imagem.length()<=3){
        return;
    }
    if((imagem.compare(0,3,"../") == 0) | (imagem.compare(0,2,"//") == 0)){
        return;
    }
    if(imagem.compare(0,1,"/")!=0){
        imagem.insert(0,"/");
    }
    char format[2000];
    memset(format,0,sizeof (format));
    strcpy(format,"GET http://");
    strcat(format,host);
    strcat(format,(char *)imagem.c_str());
    strcat(format," HTTP/1.1\r\nHost:");
    strcat(format,host);
    // strcat(format,"\r\nAccept: text/html");
    strcat(format,"\r\nConnection: close\r\n\r\n");
    // std::cout<<format;
    /* Send the request. */
    memset(&host_info, 0, sizeof(host_info));
    host_info.ai_family = AF_UNSPEC;
    host_info.ai_socktype = SOCK_STREAM;
    if (getaddrinfo(host, "http", &host_info, &host_info_list) != 0) {
        return ;
    }
    //cria um socket
    if ((idSocket = socket(host_info_list->ai_family, host_info_list->ai_socktype, host_info_list->ai_protocol)) < 0)
    {
        fprintf(stderr," Erro ao criar socket para o servidor! O programa foi encerrado\n");
        return ;
    }
    //faz a conexão
    if (connect(idSocket, host_info_list->ai_addr, host_info_list->ai_addrlen) < 0)
    {
        return ;
    }
    freeaddrinfo(host_info_list);

    int bytesSend;
    bytesSend = send (idSocket, format, strlen (format), 0);
    /* Recv data */
    memset(buffer,0,sizeof (buffer));
    int bytesRecv;
    char arquivo[1000] = "./html";
    found = imagem.find_last_of("/");
    if (found!=std::string::npos){
        std::string str = imagem.substr(0,found);
        comando+=str;
        system(comando.c_str());
    }
    strcat(arquivo,imagem.c_str());
    while ((bytesRecv = read(idSocket, &buff, 1)) > 0) {
        response.push_back(buff);
    }
    std::string msg;
    for(auto letter:response){
        msg += letter;
    }
    end_of_file =  msg.find("\r\n\r\n");
    if(end_of_file!=std::string::npos){
        msg = msg.substr(end_of_file+4);
    }
    file.open(arquivo,ofstream::binary);
    if(file.is_open()){
    file<<msg;
    file.close();
    }
}

std::set<std::string> Crawler::ExtractHyperlinks(std::string text){
    static const regex hl_regex("<a href=\"http://(.*?)\"", std::regex_constants::icase);

    return {sregex_token_iterator(text.begin(), text.end(), hl_regex, 1),
            sregex_token_iterator{}};
}

std::set<std::string> Crawler::ExtractImageslinks(std::string text){
    static const regex hl_regex("<img src=\"(.*?)\"", std::regex_constants::icase);

    return {sregex_token_iterator(text.begin(), text.end(), hl_regex, 1),
            sregex_token_iterator{}};
}

std::set<std::string> Crawler::ExtractCssLinks_1(std::string text){
    static const regex hl_regex("<link href=\"(.*?)\"", std::regex_constants::icase);

    return {sregex_token_iterator(text.begin(), text.end(), hl_regex, 1),
            sregex_token_iterator{}};
}

std::set<std::string> Crawler::ExtractCssLinks_2(std::string text){
    static const regex hl_regex("<link rel=\"stylesheet\" href=\"(.*?)\"", std::regex_constants::icase);

    return {sregex_token_iterator(text.begin(), text.end(), hl_regex, 1),
            sregex_token_iterator{}};
}

//roda o spider e o wget com profundidade 2
void Crawler::run(char *host, char *path, int nivel){
    acessados.clear();
    queue <string> first,second,third;
    std::cout<<host<<path<<"\n\e[95m";
    std::cout<<"\n"<<nivel<<"\n";
    wget(host,path, 1,nivel);
    std::cout<<"\e[0m\n\nFim spider + wget recursivo\n\n";
    acessados.clear();
}

// FAZER INTERFACE GRAFICA
