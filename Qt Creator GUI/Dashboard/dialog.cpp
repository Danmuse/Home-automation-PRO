#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    miTemporizador = new QTimer(this);
    miTemporizador->setInterval(2000);
    
    connect(miTemporizador, SIGNAL(timeout()), this, SLOT(manejarTimeOut()));
    Puerto.OpenPort();
    ui->setupUi(this);
    miTemporizador->start();

}

Dialog::~Dialog()
{
    delete ui;
}


void Dialog::on_pushButton_closeSession_clicked()
{
    miTemporizador->stop();
    Puerto.ClosePort();
    this->close();

}

void Dialog::manejarTimeOut()
{
    Information = Puerto.GetDato();
    if(Information.Info != nullptr && Information.Param != nullptr){
        Database.PushDato(Information.Param, Information.Info.toInt());
    }
    ui->TempData->setText(QString::number(Database.GetDato("Temperatura")));
}


void Dialog::on_ToggleLuz_clicked()
{
    if(ui->ToggleLuz->text() == "Prender Luz"){
        Puerto.SendData("$luz:100%");
        ui->ToggleLuz->setText("Apagar Luz");
        ui->LuzLevel->setValue(100);
    }else{
        ui->ToggleLuz->setText("Prender Luz");
        Puerto.SendData("$luz:0%");
        ui->LuzLevel->setValue(0);
    }
}


void Dialog::on_LuzLevel_actionTriggered(int action)
{
    if(action == 0){
        ui->ToggleLuz->setText("Prender Luz");
    }else if(action == 100){
        ui->ToggleLuz->setText("Apagar Luz");
    }

    Puerto.SendData(QString("$luz:%1%").arg(action));
}

