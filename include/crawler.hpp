#ifndef CRAWLER_H
#define CRAWLER_H
#include "util.hpp"

//Classe responsavel por rodar o wget e o spider
class Crawler{
public:
    Crawler();
    ~Crawler();
    std::queue <std::string> spider(char *host, char *path, int atual, int max);
    int wget (char *host,char *path, int atual, int max);
    std::set<std::string> ExtractHyperlinks(std::string text);
    void run(char *host,char *path);
    std::vector<std::string> acessados;
};
#endif