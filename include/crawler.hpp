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
    std::set<std::string> ExtractImageslinks(std::string text);
    std::set<std::string> ExtractCssLinks_1(std::string text);
    std::set<std::string> ExtractCssLinks_2(std::string text);
    void RequestImage(std::string imagem,char *host);
    void RequestCss(std::string imagem,char *host);
    void run(char *host,char *path);
    std::vector<std::string> acessados;
};
#endif