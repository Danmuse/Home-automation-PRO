#include "dialog.h"
#include "ui_dialog.h"
#include <QColorDialog>
#include <QProgressBar>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    miTemporizador = new QTimer(this);
    miTemporizador->setInterval(500);
    connect(miTemporizador, SIGNAL(timeout()), this, SLOT(manejarTimeOut()));
    ui->setupUi(this);
    miTemporizador->start();
    ui->LuzLevel->setMinimum(0);
    ui->LuzLevel->setMaximum(10);
    ui->VolumeMusic->setMinimum(0);
    ui->VolumeMusic->setMaximum(100);
    connect(ui->LuzLevel, &QSlider::valueChanged, this, &Dialog::on_LuzLevel_actionTriggered);
    connect(ui->VolumeMusic, &QSlider::valueChanged, this, &Dialog::on_VolumeMusic_actionTriggered);
    ui->Automatic->setChecked(true);
    this->on_checkBox_stateChanged(2);
    connect(ui->Automatic, &QCheckBox::stateChanged, this, &Dialog::on_checkBox_stateChanged);
    ui->ComboMusic->addItems(Canciones);
    ui->PauseMusic->setEnabled(false);
}

Dialog::~Dialog()
{
    delete ui;
}


void Dialog::on_pushButton_closeSession_clicked()
{
    miTemporizador->stop();
    this->close();

}

void Dialog::manejarTimeOut()
{
    Information = Puerto.GetDato();
    if(Information.Info != nullptr && Information.Param != nullptr ){
        Database.PushDato(Information.Param, Information.Info.toInt());
    }
}


void Dialog::on_ToggleLuz_clicked()
{
    if(ui->ToggleLuz->text() == "Prender Luz"){
        Puerto.SendData("$luz:10%");
        ui->ToggleLuz->setText("Apagar Luz");
        ui->LuzLevel->setValue(10);
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
    }else{
        ui->ToggleLuz->setText("Apagar Luz");
    }
    Puerto.SendData(QString("$luz:%1%").arg(action));
}


void Dialog::on_checkBox_stateChanged(int arg1)
{
    if(arg1 == 2){
        ui->LuzLevel->setEnabled(false);
        ui->Red->setEnabled(false);
        ui->Green->setEnabled(false);
        ui->White->setEnabled(false);
        ui->Blue->setEnabled(false);
        ui->ToggleLuz->setEnabled(false);
        Puerto.SendData(QString("$automatic:on%"));
    }else{
        ui->LuzLevel->setEnabled(true);
        ui->Red->setEnabled(true);
        ui->Green->setEnabled(true);
        ui->White->setEnabled(true);
        ui->Blue->setEnabled(true);
        ui->ToggleLuz->setEnabled(true);
        Puerto.SendData(QString("$automatic:off%"));
    }

}

void Dialog::on_PauseMusic_clicked()
{
    ui->PauseMusic->setEnabled(false);
    ui->PlayMusic->setEnabled(true);
    Puerto.SendData(QString("$pause:on%"));
}




void Dialog::on_VolumeMusic_actionTriggered(int action)
{
    if(action != 7){
        Puerto.SendData(QString("$volume:%1%").arg(action));
    }
}


void Dialog::on_PlayMusic_clicked()
{
    ui->PauseMusic->setEnabled(true);
    ui->PlayMusic->setEnabled(false);
    Puerto.SendData(QString("$song:000%1%").arg(ui->ComboMusic->currentIndex() + 1));
}


void Dialog::on_ComboMusic_currentIndexChanged(int index)
{
    ui->PauseMusic->setEnabled(false);
    ui->PlayMusic->setEnabled(true);
}


void Dialog::on_Red_clicked()
{
    Puerto.SendData(QString("$red:on%"));
}


void Dialog::on_White_clicked()
{
    Puerto.SendData(QString("$white:on%"));

}


void Dialog::on_Blue_clicked()
{
    Puerto.SendData(QString("$blue:on%"));

}


void Dialog::on_Green_clicked()
{
    Puerto.SendData(QString("$green:on%"));

}

