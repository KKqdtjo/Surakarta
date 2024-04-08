#include "result1.h"
#include "ui_result1.h"

result1::result1(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::result1)
{
    ui->setupUi(this);
    ui->label->setText("黑方获胜");
    connect(ui->exit,&QPushButton::clicked,[=](){
        this->hide();
    });
}

result1::~result1()
{
    delete ui;
}
