#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->timelable->setText("Surakarta\n启动");
    this->page2 = new Dialog;
    connect(ui->login,&QPushButton::clicked,[=](){
        this->hide();
        this->page2->show();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
