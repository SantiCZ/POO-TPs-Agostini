#include "widget.h"
#include "ui_widget.h"
#include <QApplication>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    connect(ui->pushButton, SIGNAL(pressed()), this, SLOT(slot_validar()));
    connect(ui->leclave, SIGNAL(returnPressed()), this, SLOT(slot_validar()));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::slot_validar(){
    if(ui-> leusuario->text() == "admin" && ui->leclave->text() == "1234"){
        this-> close();
    }else { ui->leclave-> clear();
}
}