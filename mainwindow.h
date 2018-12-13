#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <socket.hpp>
#include <header.hpp>
#include <crawler.hpp>
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QString doSomething();
    void InitHold();
    void Init();
    std::fstream request;
    Socket socket;
    HTTP http;
    Crawler crawler;
    int sizeBuffer = 5000;
    int newsockfd = -1;
    struct PedidoAnalisado *pedido;


private slots:
    void on_pushButton_spider_clicked();

    void on_pushButton_continue_clicked();

    void on_pushButton_browse_clicked();

    int on_spinBox_editingFinished();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
