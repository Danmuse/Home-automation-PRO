#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
QMainWindow(parent),
ui(new Ui::MainWindow) {
    ui->setupUi(this);
    puerto = nullptr;
    ui->statusLB->setText("NO CONNECTED");
    ui->statusLB->setStyleSheet("font-weight: bold; color: black; background-color: lightgray;");
    ui->selectionCOM->clear();
    QList<QSerialPortInfo> port = QSerialPortInfo::availablePorts();
    for (int index = 0; index < port.size(); index++) {
        ui->selectionCOM->addItem(port.at(index).portName());
    }
}

void MainWindow::on_pushButton_clicked() {

}

void MainWindow::on_pushButton_CONECTAR_clicked() {
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

void MainWindow::on_pushButton_ENVIAR_clicked() {
    QByteArray send;
    if (puerto) {
        send += (ui->transmitLINE->text()).toLocal8Bit();
        puerto->write(send);
        ui->transmitLINE->clear();
    }
}

void MainWindow::onDatosRecibidos() {
    QByteArray bytes;
    int cant = puerto->bytesAvailable();
    bytes.resize(cant);
    puerto->read(bytes.data(), bytes.size());
    ui->recieveLINE->insert(bytes);
    // m_datos_recibidos += bytes;
    // m_datos_recibidos = bytes;
}

MainWindow::~MainWindow() {
    puerto->close();
    delete puerto;
    delete ui;
}

