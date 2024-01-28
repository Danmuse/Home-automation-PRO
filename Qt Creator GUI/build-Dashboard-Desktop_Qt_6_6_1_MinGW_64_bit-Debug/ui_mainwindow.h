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
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *action;
    QWidget *centralwidget;
    QLabel *label_titulo;
    QLabel *label_2;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QLabel *label_password;
    QLabel *label_user;
    QLineEdit *lineEdit_user;
    QLineEdit *lineEdit_password;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_confirmLogin;
    QHBoxLayout *horizontalLayout;
    QPushButton *CreateUser;
    QPushButton *Login;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QComboBox *comboBox;
    QPushButton *pushButton;
    QLabel *PortConfirm;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        action = new QAction(MainWindow);
        action->setObjectName("action");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        label_titulo = new QLabel(centralwidget);
        label_titulo->setObjectName("label_titulo");
        label_titulo->setGeometry(QRect(300, 70, 211, 61));
        QFont font;
        font.setPointSize(20);
        label_titulo->setFont(font);
        label_titulo->setFrameShape(QFrame::Box);
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(240, 150, 421, 31));
        QFont font1;
        font1.setPointSize(12);
        label_2->setFont(font1);
        gridLayoutWidget = new QWidget(centralwidget);
        gridLayoutWidget->setObjectName("gridLayoutWidget");
        gridLayoutWidget->setGeometry(QRect(230, 190, 451, 101));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setContentsMargins(15, 0, 0, 0);
        label_password = new QLabel(gridLayoutWidget);
        label_password->setObjectName("label_password");
        QFont font2;
        font2.setPointSize(14);
        label_password->setFont(font2);
        label_password->setFrameShape(QFrame::NoFrame);

        gridLayout->addWidget(label_password, 1, 0, 1, 1);

        label_user = new QLabel(gridLayoutWidget);
        label_user->setObjectName("label_user");
        label_user->setFont(font2);
        label_user->setFrameShape(QFrame::NoFrame);

        gridLayout->addWidget(label_user, 0, 0, 1, 1);

        lineEdit_user = new QLineEdit(gridLayoutWidget);
        lineEdit_user->setObjectName("lineEdit_user");

        gridLayout->addWidget(lineEdit_user, 0, 1, 1, 1);

        lineEdit_password = new QLineEdit(gridLayoutWidget);
        lineEdit_password->setObjectName("lineEdit_password");

        gridLayout->addWidget(lineEdit_password, 1, 1, 1, 1);

        verticalLayoutWidget = new QWidget(centralwidget);
        verticalLayoutWidget->setObjectName("verticalLayoutWidget");
        verticalLayoutWidget->setGeometry(QRect(360, 300, 261, 80));
        verticalLayout_2 = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setSizeConstraint(QLayout::SetDefaultConstraint);
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        label_confirmLogin = new QLabel(verticalLayoutWidget);
        label_confirmLogin->setObjectName("label_confirmLogin");
        label_confirmLogin->setAutoFillBackground(false);
        label_confirmLogin->setFrameShape(QFrame::NoFrame);

        verticalLayout_2->addWidget(label_confirmLogin);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, -1, 0);
        CreateUser = new QPushButton(verticalLayoutWidget);
        CreateUser->setObjectName("CreateUser");
        CreateUser->setStyleSheet(QString::fromUtf8("color: black;"));

        horizontalLayout->addWidget(CreateUser);

        Login = new QPushButton(verticalLayoutWidget);
        Login->setObjectName("Login");
        Login->setBaseSize(QSize(0, 0));
        Login->setStyleSheet(QString::fromUtf8("color: black;"));

        horizontalLayout->addWidget(Login);


        verticalLayout_2->addLayout(horizontalLayout);

        layoutWidget = new QWidget(centralwidget);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(120, 70, 106, 85));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        comboBox = new QComboBox(layoutWidget);
        comboBox->setObjectName("comboBox");

        verticalLayout->addWidget(comboBox);

        pushButton = new QPushButton(layoutWidget);
        pushButton->setObjectName("pushButton");

        verticalLayout->addWidget(pushButton);

        PortConfirm = new QLabel(layoutWidget);
        PortConfirm->setObjectName("PortConfirm");
        PortConfirm->setFont(font1);
        PortConfirm->setFrameShape(QFrame::Box);

        verticalLayout->addWidget(PortConfirm);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        action->setText(QString());
        label_titulo->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p align=\"center\"><span style=\" font-size:14pt; color:#000000;\">Bienvenido a casa</span></p></body></html>", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" color:#000000;\">Por favor ingrese su usuario y contrase\303\261a</span></p></body></html>", nullptr));
        label_password->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p align=\"center\"><span style=\" color:#000000;\">Contrase\303\261a</span></p></body></html>", nullptr));
        label_user->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p align=\"center\"><span style=\" color:#000000;\">Usuario</span></p></body></html>", nullptr));
        label_confirmLogin->setText(QString());
        CreateUser->setText(QCoreApplication::translate("MainWindow", "Crear un usuario", nullptr));
        Login->setText(QCoreApplication::translate("MainWindow", "Login", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "Conectar", nullptr));
        PortConfirm->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p align=\"center\">desconectado</p></body></html>", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
