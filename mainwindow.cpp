#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "socket.hpp"
#include "header.hpp"
#include "util.hpp"


int total_de_bits_recebidos = 0;
int v = 0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Init()
{
    std::fstream request;
    int sizeBuffer = 5000;
    int opcao;
    char buffer[sizeBuffer];
    std::string go;
    //Cria socket
    socket.bind_socket = socket.SocketCreate();
    if (socket.bind_socket == -1){
        printf("Could not create socket");
        return ;
    }
    printf("Socket created\n");
    //Faz o Bind e o Listen do socket
    socket.SocketBind();
    if( socket.bind_socket < 0){
        //print the error message
        printf("bind failed.");
        return ;
    }
    printf("Bind e Listen feito \n");
}

void MainWindow::InitHold()
{
    std::cout<<"init hold";
    socket.inSocket = accept(socket.bind_socket, nullptr, nullptr);
    request.open("request",std::ios::out);
    int recvd;
    char buffer[sizeBuffer];
    newsockfd = socket.inSocket;
    char *mensagem;  // armazena a mensagem da URL

    mensagem = (char *) malloc(sizeBuffer);

    mensagem[0] = '\0';
    // determina onde é o fim da request


    while (strstr(mensagem, "\r\n\r\n") == NULL) {

        recvd = recv(newsockfd, buffer, sizeBuffer, 0) ;

        if(recvd < 0 ){
            fprintf(stderr," Erro ao receber mensagem do cliente!\n");
            exit (1);
        }else if(recvd == 0) {
            break;
        } else {
            total_de_bits_recebidos += recvd;
            //Se o tamanho da mensagem for maior que o tamanho da string buffer, dobra o tamanho da string
            buffer[recvd] = '\0';
            if (total_de_bits_recebidos > sizeBuffer) {
                sizeBuffer *= 2;
                mensagem = (char *) realloc(mensagem, sizeBuffer);
                if (mensagem == NULL) {
                    fprintf(stderr," Erro durante a realocação de memoria!\n");
                    exit (1);
                }
            }
        }
        strcat(mensagem, buffer);
    }
    std::string msg(mensagem);
    request << mensagem;
    request.close();
    strcpy(mensagem, msg.c_str());
    if(strlen(mensagem) <= 0){
        fprintf(stderr, "Erro na mensagem");
        return;
    }

    // cria o pedido analisado, struct com as informacoes das requests
//    struct PedidoAnalisado *pedido;
    pedido = http.PedidoAnalisado_create();
    ui->label->setText(mensagem);
    http.Analise_do_pedido(pedido, mensagem, strlen(mensagem));
    std::cout<<"pedido host:"<<pedido->host;
    return;
}

void MainWindow::on_pushButton_spider_clicked(){
    int v = 3;
//    int v = ui->spinBox->value();
    std::cout<<v;
//    std::cout<<"pedido host:9898"<<pedido->host;
//    crawler.run(pedido->host, pedido->path, v);
    InitHold();
    return;
}

void MainWindow::on_pushButton_continue_clicked()
{
    exit(-1);
    this->InitHold();
    return;
}

void MainWindow::on_pushButton_browse_clicked()
{
    std::string temp = "a";
    int iServerfd;
    char *browser_request;
    std::cout<<"\n browser\npedido host:"<<pedido->host;
    if (pedido->port == NULL) pedido->port = (char *) "80";
    try{
        browser_request  = http.converte_Request_to_string(pedido);
        iServerfd = socket.createServerSocket(pedido->host, pedido->port);
        socket.sendToServerSocket(browser_request, iServerfd, total_de_bits_recebidos);
        temp = socket.receiveFromServer(newsockfd, iServerfd);
        std::cout<<"\e[0m\n";
        ui->label_2->setText((char *)temp.c_str());
        http.PedidoAnalisado_destroy(pedido);
    }
    catch(char const* erro){
        std::cout<<"\n"<<erro<<"\n";
    }
    this->InitHold();
    return;
}

int MainWindow::on_spinBox_editingFinished()
{
    int v = ui->spinBox->value();
    return v;
}
