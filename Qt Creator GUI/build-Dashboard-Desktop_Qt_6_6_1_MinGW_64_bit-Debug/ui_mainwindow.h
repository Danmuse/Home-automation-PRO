/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *action;
    QWidget *centralwidget;
    QLabel *label_user;
    QLabel *label_titulo;
    QLabel *label_confirmLogin;
    QLineEdit *lineEdit_user;
    QLabel *label_password;
    QPushButton *pushButton_login;
    QLabel *label_2;
    QLineEdit *lineEdit_password;
    QComboBox *comboBox;
    QPushButton *pushButton;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        action = new QAction(MainWindow);
        action->setObjectName("action");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        label_user = new QLabel(centralwidget);
        label_user->setObjectName("label_user");
        label_user->setGeometry(QRect(160, 230, 131, 41));
        QFont font;
        font.setPointSize(14);
        label_user->setFont(font);
        label_user->setFrameShape(QFrame::Box);
        label_titulo = new QLabel(centralwidget);
        label_titulo->setObjectName("label_titulo");
        label_titulo->setGeometry(QRect(340, 40, 211, 61));
        QFont font1;
        font1.setPointSize(20);
        label_titulo->setFont(font1);
        label_titulo->setFrameShape(QFrame::Box);
        label_confirmLogin = new QLabel(centralwidget);
        label_confirmLogin->setObjectName("label_confirmLogin");
        label_confirmLogin->setGeometry(QRect(360, 420, 281, 31));
        label_confirmLogin->setFrameShape(QFrame::Box);
        lineEdit_user = new QLineEdit(centralwidget);
        lineEdit_user->setObjectName("lineEdit_user");
        lineEdit_user->setGeometry(QRect(370, 230, 271, 41));
        label_password = new QLabel(centralwidget);
        label_password->setObjectName("label_password");
        label_password->setGeometry(QRect(160, 320, 131, 51));
        label_password->setFont(font);
        label_password->setFrameShape(QFrame::Box);
        pushButton_login = new QPushButton(centralwidget);
        pushButton_login->setObjectName("pushButton_login");
        pushButton_login->setGeometry(QRect(370, 480, 191, 61));
        pushButton_login->setStyleSheet(QString::fromUtf8("color: black;"));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(240, 150, 421, 31));
        QFont font2;
        font2.setPointSize(12);
        label_2->setFont(font2);
        lineEdit_password = new QLineEdit(centralwidget);
        lineEdit_password->setObjectName("lineEdit_password");
        lineEdit_password->setGeometry(QRect(370, 330, 271, 41));
        comboBox = new QComboBox(centralwidget);
        comboBox->setObjectName("comboBox");
        comboBox->setGeometry(QRect(100, 40, 71, 31));
        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(100, 80, 80, 24));
        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        action->setText(QString());
        label_user->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" color:#000000;\">Usuario</span></p></body></html>", nullptr));
        label_titulo->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:14pt; color:#000000;\">Bienvenido a casa</span></p></body></html>", nullptr));
        label_confirmLogin->setText(QString());
        label_password->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" color:#000000;\">Contrase\303\261a</span></p></body></html>", nullptr));
        pushButton_login->setText(QCoreApplication::translate("MainWindow", "Login", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" color:#000000;\">Por favor ingrese su usuario y contrase\303\261a</span></p></body></html>", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "Conectar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
