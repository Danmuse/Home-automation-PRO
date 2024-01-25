/********************************************************************************
** Form generated from reading UI file 'information.ui'
**
** Created by: Qt User Interface Compiler version 6.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INFORMATION_H
#define UI_INFORMATION_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Information
{
public:
    QPushButton *pushButton_closeSession;
    QPushButton *pushButton_conectar;
    QLabel *labe_tittle;
    QLabel *label_temp;
    QLineEdit *lineEdit_temp;
    QLabel *label_lightLevel;
    QLineEdit *lineEdit_lightLevel;

    void setupUi(QDialog *Information)
    {
        if (Information->objectName().isEmpty())
            Information->setObjectName("Information");
        Information->resize(1025, 686);
        pushButton_closeSession = new QPushButton(Information);
        pushButton_closeSession->setObjectName("pushButton_closeSession");
        pushButton_closeSession->setGeometry(QRect(630, 530, 121, 51));
        pushButton_conectar = new QPushButton(Information);
        pushButton_conectar->setObjectName("pushButton_conectar");
        pushButton_conectar->setGeometry(QRect(380, 530, 121, 51));
        labe_tittle = new QLabel(Information);
        labe_tittle->setObjectName("labe_tittle");
        labe_tittle->setGeometry(QRect(410, 150, 281, 81));
        QFont font;
        font.setPointSize(16);
        labe_tittle->setFont(font);
        labe_tittle->setFrameShape(QFrame::Box);
        label_temp = new QLabel(Information);
        label_temp->setObjectName("label_temp");
        label_temp->setGeometry(QRect(260, 290, 141, 51));
        QFont font1;
        font1.setPointSize(12);
        label_temp->setFont(font1);
        label_temp->setFrameShape(QFrame::Panel);
        lineEdit_temp = new QLineEdit(Information);
        lineEdit_temp->setObjectName("lineEdit_temp");
        lineEdit_temp->setGeometry(QRect(470, 300, 271, 41));
        label_lightLevel = new QLabel(Information);
        label_lightLevel->setObjectName("label_lightLevel");
        label_lightLevel->setGeometry(QRect(260, 390, 141, 51));
        label_lightLevel->setFont(font1);
        label_lightLevel->setFrameShape(QFrame::Panel);
        lineEdit_lightLevel = new QLineEdit(Information);
        lineEdit_lightLevel->setObjectName("lineEdit_lightLevel");
        lineEdit_lightLevel->setGeometry(QRect(470, 390, 271, 41));

        retranslateUi(Information);

        QMetaObject::connectSlotsByName(Information);
    } // setupUi

    void retranslateUi(QDialog *Information)
    {
        Information->setWindowTitle(QCoreApplication::translate("Information", "Dialog", nullptr));
        pushButton_closeSession->setText(QCoreApplication::translate("Information", "Cerrar Sesi\303\263n", nullptr));
        pushButton_conectar->setText(QCoreApplication::translate("Information", "Conectar", nullptr));
        labe_tittle->setText(QCoreApplication::translate("Information", "Informaci\303\263n del hogar", nullptr));
        label_temp->setText(QCoreApplication::translate("Information", "Temperatura", nullptr));
        label_lightLevel->setText(QCoreApplication::translate("Information", "Luminosidad", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Information: public Ui_Information {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INFORMATION_H
