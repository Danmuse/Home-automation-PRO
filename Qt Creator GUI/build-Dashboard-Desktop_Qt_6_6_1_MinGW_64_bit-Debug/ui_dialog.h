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
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
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
    QPushButton *pushButton_closeSession;
    QPushButton *ToggleLuz;
    QSlider *LuzLevel;
    QLabel *labe_tittle_2;
    QCheckBox *Automatic;
    QFrame *frame;
    QWidget *layoutWidget;
    QGridLayout *gridLayout;
    QPushButton *Red;
    QPushButton *White;
    QPushButton *Blue;
    QPushButton *Green;
    QLabel *labe_tittle_3;
    QWidget *verticalLayoutWidget_2;
    QVBoxLayout *verticalLayout_2;
    QComboBox *ComboMusic;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *PlayMusic;
    QPushButton *PauseMusic;
    QSlider *VolumeMusic;
    QLabel *label_titulo;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName("Dialog");
        Dialog->resize(720, 514);
        label_lightLevel = new QLabel(Dialog);
        label_lightLevel->setObjectName("label_lightLevel");
        label_lightLevel->setGeometry(QRect(50, 150, 131, 31));
        QFont font;
        font.setPointSize(14);
        label_lightLevel->setFont(font);
        label_lightLevel->setFrameShape(QFrame::NoFrame);
        pushButton_closeSession = new QPushButton(Dialog);
        pushButton_closeSession->setObjectName("pushButton_closeSession");
        pushButton_closeSession->setGeometry(QRect(500, 60, 91, 31));
        ToggleLuz = new QPushButton(Dialog);
        ToggleLuz->setObjectName("ToggleLuz");
        ToggleLuz->setGeometry(QRect(50, 260, 81, 31));
        LuzLevel = new QSlider(Dialog);
        LuzLevel->setObjectName("LuzLevel");
        LuzLevel->setGeometry(QRect(50, 230, 211, 16));
        LuzLevel->setOrientation(Qt::Horizontal);
        labe_tittle_2 = new QLabel(Dialog);
        labe_tittle_2->setObjectName("labe_tittle_2");
        labe_tittle_2->setGeometry(QRect(500, 150, 91, 31));
        labe_tittle_2->setFont(font);
        labe_tittle_2->setFrameShape(QFrame::NoFrame);
        labe_tittle_2->setTextInteractionFlags(Qt::LinksAccessibleByMouse);
        Automatic = new QCheckBox(Dialog);
        Automatic->setObjectName("Automatic");
        Automatic->setGeometry(QRect(50, 180, 111, 41));
        frame = new QFrame(Dialog);
        frame->setObjectName("frame");
        frame->setGeometry(QRect(300, 190, 101, 81));
        frame->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        layoutWidget = new QWidget(frame);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(10, 10, 80, 56));
        gridLayout = new QGridLayout(layoutWidget);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setContentsMargins(0, 0, 0, 0);
        Red = new QPushButton(layoutWidget);
        Red->setObjectName("Red");
        Red->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 0, 0);"));

        gridLayout->addWidget(Red, 0, 0, 1, 1);

        White = new QPushButton(layoutWidget);
        White->setObjectName("White");
        White->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));

        gridLayout->addWidget(White, 0, 1, 1, 1);

        Blue = new QPushButton(layoutWidget);
        Blue->setObjectName("Blue");
        Blue->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 255);"));

        gridLayout->addWidget(Blue, 1, 0, 1, 1);

        Green = new QPushButton(layoutWidget);
        Green->setObjectName("Green");
        Green->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 255, 0);"));

        gridLayout->addWidget(Green, 1, 1, 1, 1);

        labe_tittle_3 = new QLabel(Dialog);
        labe_tittle_3->setObjectName("labe_tittle_3");
        labe_tittle_3->setGeometry(QRect(300, 150, 91, 31));
        labe_tittle_3->setFont(font);
        labe_tittle_3->setFrameShape(QFrame::NoFrame);
        labe_tittle_3->setTextInteractionFlags(Qt::LinksAccessibleByMouse);
        verticalLayoutWidget_2 = new QWidget(Dialog);
        verticalLayoutWidget_2->setObjectName("verticalLayoutWidget_2");
        verticalLayoutWidget_2->setGeometry(QRect(480, 190, 191, 91));
        verticalLayout_2 = new QVBoxLayout(verticalLayoutWidget_2);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        ComboMusic = new QComboBox(verticalLayoutWidget_2);
        ComboMusic->setObjectName("ComboMusic");

        verticalLayout_2->addWidget(ComboMusic);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        PlayMusic = new QPushButton(verticalLayoutWidget_2);
        PlayMusic->setObjectName("PlayMusic");
        QIcon icon;
        icon.addFile(QString::fromUtf8("568288.png"), QSize(), QIcon::Normal, QIcon::Off);
        PlayMusic->setIcon(icon);

        horizontalLayout->addWidget(PlayMusic);

        PauseMusic = new QPushButton(verticalLayoutWidget_2);
        PauseMusic->setObjectName("PauseMusic");
        QIcon icon1;
        icon1.addFile(QString::fromUtf8("pause.png"), QSize(), QIcon::Normal, QIcon::Off);
        PauseMusic->setIcon(icon1);

        horizontalLayout->addWidget(PauseMusic);


        verticalLayout->addLayout(horizontalLayout);

        VolumeMusic = new QSlider(verticalLayoutWidget_2);
        VolumeMusic->setObjectName("VolumeMusic");
        VolumeMusic->setOrientation(Qt::Horizontal);

        verticalLayout->addWidget(VolumeMusic);


        verticalLayout_2->addLayout(verticalLayout);

        label_titulo = new QLabel(Dialog);
        label_titulo->setObjectName("label_titulo");
        label_titulo->setGeometry(QRect(240, 50, 221, 61));
        QFont font1;
        font1.setPointSize(20);
        label_titulo->setFont(font1);
        label_titulo->setFrameShape(QFrame::Box);

        retranslateUi(Dialog);

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QCoreApplication::translate("Dialog", "Dialog", nullptr));
        label_lightLevel->setText(QCoreApplication::translate("Dialog", "Luminosidad", nullptr));
        pushButton_closeSession->setText(QCoreApplication::translate("Dialog", "Cerrar Sesi\303\263n", nullptr));
        ToggleLuz->setText(QCoreApplication::translate("Dialog", "Prender Luz", nullptr));
        labe_tittle_2->setText(QCoreApplication::translate("Dialog", "Cancion", nullptr));
        Automatic->setText(QCoreApplication::translate("Dialog", "Luz Automatica", nullptr));
        Red->setText(QString());
        White->setText(QString());
        Blue->setText(QString());
        Green->setText(QString());
        labe_tittle_3->setText(QCoreApplication::translate("Dialog", "Colores", nullptr));
        PlayMusic->setText(QCoreApplication::translate("Dialog", "Reproducir", nullptr));
        PauseMusic->setText(QCoreApplication::translate("Dialog", "Pausar", nullptr));
        label_titulo->setText(QCoreApplication::translate("Dialog", "<html><head/><body><p align=\"center\"><span style=\" font-size:14pt; color:#000000;\">Informacion del hogar</span></p></body></html>", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_H
