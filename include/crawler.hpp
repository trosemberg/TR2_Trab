#ifndef CRAWLER_H
#define CRAWLER_H
#include "util.hpp"

//Classe responsavel por rodar o wget e o spider
class Crawler{
public:
    Crawler();
    ~Crawler();
    int compara(std::queue <std::string> gq, std::string name);
    std::queue <std::string> spider(char *host, char *path);
    int wget (char *host,char *path);
    void run(char *host,char *path);
};
#endif