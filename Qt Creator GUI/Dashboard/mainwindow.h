#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QtSerialPort>
#include <QSerialPort>
#include "qcombobox.h"
#include "programconfig.h"
#include "dialog.h"
#include "validatormodal.h"
#include <QMovie>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void inicializateTimer(QTimer *timer, void (MainWindow::*func)());
    void CancelateTimer(QTimer *timer);


private slots:

    void PortsAvailable();

    void verificatePort();

    void on_Login_clicked();

    void on_pushButton_clicked();

    void on_CreateUser_clicked();

    void CreateUserEnable();

    void WaitingConnect();

private:
    Ui::MainWindow *ui;
    Dialog *Dial;
    ValidatorModal *Validator;
    QTimer* TimerPort;
    SerialParams Confirm;
    QMovie *movie;
    QLabel *label;
    QTimer* PuertoConnectTimer;
};

#endif // MAINWINDOW_H
