#include "programconfig.h"
#include "mainwindow.h"


#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Login");
    w.show();
    /*

    //int valor = data.GetDato("Temperatura");
    //qDebug() << "data:" << data.VerificateUser("hola");

    if(data.LoginUser("hola", "hola")){
        qDebug() << "Funciono";
    }else{
        qDebug() << "No Funciono";
    }
    //data.PushDato("Temperatura", 40);
    */
    /*
    QList<QSerialPortInfo> portList = QSerialPortInfo::availablePorts();

    qDebug() << "Puertos Serie Disponibles:";
    for (const QSerialPortInfo &portInfo : portList) {
        qDebug() << "Nombre:" << portInfo.portName();
        qDebug() << "Descripción:" << portInfo.description();
        qDebug() << "Fabricante:" << portInfo.manufacturer();
        qDebug() << "-------------------------------------";
    }
    */
    return a.exec();
    qDebug() << "Termino";

}
