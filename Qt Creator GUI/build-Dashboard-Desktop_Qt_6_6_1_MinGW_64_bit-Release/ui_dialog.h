/********************************************************************************
** Form generated from reading UI file 'dialog.ui'
**
** Created by: Qt User Interface Compiler version 6.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_H
#define UI_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QLabel *label_lightLevel;
    QLabel *labe_tittle;
    QPushButton *pushButton_closeSession;
    QLabel *label_temp;
    QPushButton *ToggleLuz;
    QSlider *LuzLevel;
    QLabel *TempData;
    QLabel *labe_tittle_2;
    QCheckBox *checkBox;
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QComboBox *comboBox;
    QPushButton *pushButton;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName("Dialog");
        Dialog->resize(720, 514);
        label_lightLevel = new QLabel(Dialog);
        label_lightLevel->setObjectName("label_lightLevel");
        label_lightLevel->setGeometry(QRect(40, 150, 131, 31));
        QFont font;
        font.setPointSize(14);
        label_lightLevel->setFont(font);
        label_lightLevel->setFrameShape(QFrame::NoFrame);
        labe_tittle = new QLabel(Dialog);
        labe_tittle->setObjectName("labe_tittle");
        labe_tittle->setGeometry(QRect(190, 40, 281, 81));
        QFont font1;
        font1.setPointSize(16);
        labe_tittle->setFont(font1);
        labe_tittle->setFrameShape(QFrame::Box);
        pushButton_closeSession = new QPushButton(Dialog);
        pushButton_closeSession->setObjectName("pushButton_closeSession");
        pushButton_closeSession->setGeometry(QRect(480, 410, 121, 51));
        label_temp = new QLabel(Dialog);
        label_temp->setObjectName("label_temp");
        label_temp->setGeometry(QRect(290, 230, 141, 51));
        QFont font2;
        font2.setPointSize(12);
        label_temp->setFont(font2);
        label_temp->setFrameShape(QFrame::Panel);
        ToggleLuz = new QPushButton(Dialog);
        ToggleLuz->setObjectName("ToggleLuz");
        ToggleLuz->setGeometry(QRect(40, 260, 81, 31));
        LuzLevel = new QSlider(Dialog);
        LuzLevel->setObjectName("LuzLevel");
        LuzLevel->setGeometry(QRect(40, 230, 211, 16));
        LuzLevel->setOrientation(Qt::Horizontal);
        TempData = new QLabel(Dialog);
        TempData->setObjectName("TempData");
        TempData->setGeometry(QRect(290, 280, 141, 51));
        TempData->setFont(font2);
        TempData->setFrameShape(QFrame::Panel);
        labe_tittle_2 = new QLabel(Dialog);
        labe_tittle_2->setObjectName("labe_tittle_2");
        labe_tittle_2->setGeometry(QRect(490, 150, 91, 31));
        QFont font3;
        font3.setPointSize(11);
        labe_tittle_2->setFont(font3);
        labe_tittle_2->setFrameShape(QFrame::NoFrame);
        labe_tittle_2->setTextInteractionFlags(Qt::LinksAccessibleByMouse);
        checkBox = new QCheckBox(Dialog);
        checkBox->setObjectName("checkBox");
        checkBox->setGeometry(QRect(40, 180, 111, 41));
        widget = new QWidget(Dialog);
        widget->setObjectName("widget");
        widget->setGeometry(QRect(490, 190, 151, 56));
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        comboBox = new QComboBox(widget);
        comboBox->setObjectName("comboBox");

        verticalLayout->addWidget(comboBox);

        pushButton = new QPushButton(widget);
        pushButton->setObjectName("pushButton");

        verticalLayout->addWidget(pushButton);


        retranslateUi(Dialog);

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QCoreApplication::translate("Dialog", "Dialog", nullptr));
        label_lightLevel->setText(QCoreApplication::translate("Dialog", "Luminosidad", nullptr));
        labe_tittle->setText(QCoreApplication::translate("Dialog", "Informaci\303\263n del hogar", nullptr));
        pushButton_closeSession->setText(QCoreApplication::translate("Dialog", "Cerrar Sesi\303\263n", nullptr));
        label_temp->setText(QCoreApplication::translate("Dialog", "Temperatura", nullptr));
        ToggleLuz->setText(QCoreApplication::translate("Dialog", "Prender Luz", nullptr));
        TempData->setText(QCoreApplication::translate("Dialog", "0", nullptr));
        labe_tittle_2->setText(QCoreApplication::translate("Dialog", "Cancion", nullptr));
        checkBox->setText(QCoreApplication::translate("Dialog", "Luz Automatica", nullptr));
        pushButton->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_H
