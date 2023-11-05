#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QtSerialPort>
#include <QSerialPort>
#include "qcombobox.h"
#include "qserialport.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_CONECTAR_clicked();
    void on_pushButton_ENVIAR_clicked();
    void onDatosRecibidos();

private:
    Ui::MainWindow *ui;
    QSerialPort *puerto;
};

#endif // MAINWINDOW_H
