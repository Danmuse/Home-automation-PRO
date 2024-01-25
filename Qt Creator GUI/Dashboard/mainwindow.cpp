#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
QMainWindow(parent),
ui(new Ui::MainWindow) {
    ui->setupUi(this);
    QList<QSerialPortInfo> Puertos = Puerto.GetTotalsPorts();
    for (int index = 0; index < Puertos.size(); index++) {
        ui->comboBox->addItem(Puertos.at(index).portName());
    }

    //if(Puertos.size() > 0){
    //    Puerto.SetPort(Puertos.at(0).portName());
    //}else{
    //    qDebug() << "No hay un puerto disponible";
    //}

}

MainWindow::~MainWindow() {
    Puerto.ClosePort();
    //delete Puerto;
    delete ui;

    /*
    puerto->close();
    delete puerto;
    delete ui;
    */
}


/*
void MainWindow::onDatosRecibidos() {

    QByteArray bytes;
   // int cant = m_Puerto->m_Puerto->bytesAvailable();
   //bytes.resize(cant);
   // m_Puerto->m_Puerto->read(bytes.data(), bytes.size());
   // ui->recieveLINE->insert(bytes);
    // m_datos_recibidos += bytes;
    // m_datos_recibidos = bytes;

}
*/




/*
void MainWindow::on_connectBTN_clicked()
{

    if(Puerto.GetPortStatus()){
        Puerto.ClosePort();
        //delete Puerto;
        //Puerto = nullptr;
        //ui->statusLB->setStyleSheet("font-weight: bold; color: black; background-color: lightgray;");
       //ui->statusLB->setText("NO CONNECTED");
        //ui->connectBTN->setText("CONNECT");
    }else{
        //m_Puerto->SetPort(ui->selectionCOM->currentText());
        if(Puerto.OpenPort()){
            //ui->statusLB->setStyleSheet("font-weight: bold; color: black; background-color: lightgreen;");
            //ui->statusLB->setText("CONNECTED");
            //ui->connectBTN->setText("DISCONNECT");
        }
    }


    if (!puerto) {
        puerto = new QSerialPort(ui->selectionCOM->currentText());
        puerto->setBaudRate(QSerialPort::Baud9600);
        puerto->setDataBits(QSerialPort::Data8);
        puerto->setFlowControl(QSerialPort::NoFlowControl);
        puerto->setStopBits(QSerialPort::OneStop);
        puerto->setParity(QSerialPort::NoParity);

        if (puerto->open(QIODevice::ReadWrite)) {
            ui->statusLB->setStyleSheet("font-weight: bold; color: black; background-color: lightgreen;");
            ui->statusLB->setText("CONNECTED");
            ui->connectBTN->setText("DISCONNECT");
            connect(puerto, SIGNAL(readyRead()), this, SLOT(onDatosRecibidos()));
        }
    } else {
        puerto->close();
        delete puerto;
        puerto = nullptr;
        ui->statusLB->setStyleSheet("font-weight: bold; color: black; background-color: lightgray;");
        ui->statusLB->setText("NO CONNECTED");
        ui->connectBTN->setText("CONNECT");
    }

}
*/
/*
void MainWindow::on_sendBTN_clicked()
{
    QByteArray send;
    if (Puerto.GetPortStatus()) {
        //send += (ui->transmitLINE->text()).toLocal8Bit();
        if(Puerto.SendData(send)){
            //ui->transmitLINE->clear();
        }else{
            qDebug() << "Error";
        }
    }
}
*/




void MainWindow::on_pushButton_login_clicked()
{
    Dialog *Dial = new Dialog(this);

    QString Usuario = ui->lineEdit_user->text();
    QString Contraseña = ui->lineEdit_password->text();
    if(Database.LoginUser(Usuario, Contraseña)){
        ui->lineEdit_password->clear();
        ui->lineEdit_user->clear();
        Dial->show();
    }else{
        ui->label_confirmLogin->setText("Usuario o Contraseña incorrectos");
    }
}

void MainWindow::on_pushButton_clicked()
{
    Puerto.SetPort(ui->comboBox->currentText());
}

