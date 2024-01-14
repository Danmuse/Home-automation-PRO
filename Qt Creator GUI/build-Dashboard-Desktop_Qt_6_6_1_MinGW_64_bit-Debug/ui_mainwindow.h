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
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QLineEdit *transmitLINE;
    QLineEdit *recieveLINE;
    QPushButton *sendBTN;
    QPushButton *connectBTN;
    QLabel *statusLB;
    QComboBox *selectionCOM;
    QPushButton *exitBTN;
    QLabel *txLB;
    QLabel *rxLB;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        transmitLINE = new QLineEdit(centralwidget);
        transmitLINE->setObjectName("transmitLINE");
        transmitLINE->setGeometry(QRect(40, 80, 113, 25));
        recieveLINE = new QLineEdit(centralwidget);
        recieveLINE->setObjectName("recieveLINE");
        recieveLINE->setGeometry(QRect(40, 140, 113, 25));
        sendBTN = new QPushButton(centralwidget);
        sendBTN->setObjectName("sendBTN");
        sendBTN->setGeometry(QRect(200, 130, 89, 25));
        connectBTN = new QPushButton(centralwidget);
        connectBTN->setObjectName("connectBTN");
        connectBTN->setGeometry(QRect(340, 130, 89, 25));
        statusLB = new QLabel(centralwidget);
        statusLB->setObjectName("statusLB");
        statusLB->setGeometry(QRect(180, 80, 131, 31));
        statusLB->setLineWidth(3);
        statusLB->setAlignment(Qt::AlignCenter);
        selectionCOM = new QComboBox(centralwidget);
        selectionCOM->setObjectName("selectionCOM");
        selectionCOM->setGeometry(QRect(340, 80, 86, 25));
        exitBTN = new QPushButton(centralwidget);
        exitBTN->setObjectName("exitBTN");
        exitBTN->setGeometry(QRect(340, 200, 89, 25));
        txLB = new QLabel(centralwidget);
        txLB->setObjectName("txLB");
        txLB->setGeometry(QRect(40, 50, 111, 21));
        QFont font;
        font.setPointSize(15);
        txLB->setFont(font);
        txLB->setLineWidth(3);
        txLB->setAlignment(Qt::AlignCenter);
        rxLB = new QLabel(centralwidget);
        rxLB->setObjectName("rxLB");
        rxLB->setGeometry(QRect(40, 110, 111, 21));
        rxLB->setFont(font);
        rxLB->setLineWidth(3);
        rxLB->setMidLineWidth(0);
        rxLB->setAlignment(Qt::AlignCenter);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        transmitLINE->setText(QCoreApplication::translate("MainWindow", "QLineEdit", nullptr));
        recieveLINE->setText(QCoreApplication::translate("MainWindow", "QLineEdit", nullptr));
        sendBTN->setText(QCoreApplication::translate("MainWindow", "ENVIAR", nullptr));
        connectBTN->setText(QCoreApplication::translate("MainWindow", "CONECTAR", nullptr));
        statusLB->setText(QCoreApplication::translate("MainWindow", "ESTADO", nullptr));
        exitBTN->setText(QCoreApplication::translate("MainWindow", "SALIR", nullptr));
        txLB->setText(QCoreApplication::translate("MainWindow", "Transmisi\303\263n", nullptr));
        rxLB->setText(QCoreApplication::translate("MainWindow", "Recepci\303\263n", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
