#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QtSerialPort>
#include <QSerialPort>
#include "qcombobox.h"
#include "programconfig.h"
#include "dialog.h"
#include "validatormodal.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:


    void on_Login_clicked();

    void on_pushButton_clicked();

    void on_CreateUser_clicked();

    void CreateUserEnable();

private:
    Ui::MainWindow *ui;
    Dialog *Dial;
    ValidatorModal *Validator;
};

#endif // MAINWINDOW_H
