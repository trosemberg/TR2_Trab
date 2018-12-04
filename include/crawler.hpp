#ifndef CRAWLER_H
#define CRAWLER_H
#include "util.hpp"
class Crawler{
public:
    Crawler();
    ~Crawler();
    int wget (char *host);
    void delay (float delay1);
    void showq(std::queue <std::string> gq);
    int compara(std::queue <std::string> gq, std::string name);
    void spider(char *host);
};
#endif