#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include<QLabel>
#include<QPixmap>
#include<QLineEdit>
#include<QFont>
#include<QPalette>
#include<QFrame>
#include<QDebug>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QFont font;
    font.setFamily("汉仪文黑-85W");
    setWindowTitle("游戏登录界面");
    QPushButton * btn1 =new QPushButton;
    btn1->setParent(this);
    btn1->setText("登录");
    btn1->setFont(font);
    QPushButton * btn2 = new QPushButton("取消",this);
    btn2->setFont(font);
    btn1->move(100,180);
    btn1->resize(80,20);
    btn2->move(220,180);
    btn2->resize(80,20);
    setFixedSize(425,250);//固定窗口大小
    QLabel* lb1 = new QLabel(this);
    lb1->setParent(this);
    lb1->setText("用户：");
    lb1->setFont(font);
    lb1->resize(50,20);
    lb1->move(100,80);
    QLabel* lb2 = new QLabel(this);
    lb2->setParent(this);
    lb2->setFont(font);
    lb2->setText("密码：");
    lb2->resize(50,20);
    lb2->move(100,110);
    QLineEdit * edit1 =new QLineEdit(this);
    edit1->setParent(this);
    edit1->resize(150,20);
    edit1->move(150,80);
    QLineEdit * edit2 =new QLineEdit(this);
    edit2->setParent(this);
    edit2->resize(150,20);
    edit2->move(150,110);
    edit2->setEchoMode(QLineEdit::Password);
    connect(btn2, &QPushButton::clicked, this, &QWidget::close);
    this->page2 = new Dialog;
    connect(btn1,&QPushButton::clicked,[=](){
        this->hide();
        this->page2->show();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
