#include "information.h"
#include "Ui_Information.h"

information::information(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::information)
{
    ui->setupUi(this);
}

information::~information()
{
    delete ui;
}

void information::on_pushButton_closeSession_clicked()
{
    this->close();
}

