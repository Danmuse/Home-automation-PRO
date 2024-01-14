#include "mainwindow.h"
#include "database.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    /*MainWindow w;
    w.show();*/
    database data("brb0vzvtpbxxloikqzji-mysql.services.clever-cloud.com",
                  "uaujnwpmnmd9mwxh",
                  "QMYSQL",
                  3306,
                  "PXfKRV9ZeWx3UVFUpJ3K",
                  "brb0vzvtpbxxloikqzji");
    //int valor = data.GetDato("Temperatura");
    //qDebug() << "data:" << data.VerificateUser("hola");

    if(data.LoginUser("hola", "hola")){
        qDebug() << "Funciono";
    }else{
        qDebug() << "No Funciono";
    }
    //data.PushDato("Temperatura", 40);
    while(1){

    }

    return a.exec();
}
