/********************************************************************************
** Form generated from reading UI file 'validatormodal.ui'
**
** Created by: Qt User Interface Compiler version 6.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VALIDATORMODAL_H
#define UI_VALIDATORMODAL_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ValidatorModal
{
public:
    QLabel *TitleValidator;
    QLabel *TextValidator;
    QLabel *labelText;
    QPushButton *ButtonValidator;
    QLineEdit *CodeText;

    void setupUi(QWidget *ValidatorModal)
    {
        if (ValidatorModal->objectName().isEmpty())
            ValidatorModal->setObjectName("ValidatorModal");
        ValidatorModal->resize(400, 300);
        TitleValidator = new QLabel(ValidatorModal);
        TitleValidator->setObjectName("TitleValidator");
        TitleValidator->setGeometry(QRect(90, 30, 221, 61));
        QFont font;
        font.setPointSize(20);
        TitleValidator->setFont(font);
        TitleValidator->setFrameShape(QFrame::Box);
        TextValidator = new QLabel(ValidatorModal);
        TextValidator->setObjectName("TextValidator");
        TextValidator->setGeometry(QRect(60, 180, 281, 41));
        TextValidator->setFont(font);
        TextValidator->setFrameShape(QFrame::NoFrame);
        labelText = new QLabel(ValidatorModal);
        labelText->setObjectName("labelText");
        labelText->setGeometry(QRect(60, 100, 281, 31));
        labelText->setFont(font);
        labelText->setFrameShape(QFrame::Box);
        ButtonValidator = new QPushButton(ValidatorModal);
        ButtonValidator->setObjectName("ButtonValidator");
        ButtonValidator->setGeometry(QRect(260, 230, 91, 31));
        CodeText = new QLineEdit(ValidatorModal);
        CodeText->setObjectName("CodeText");
        CodeText->setGeometry(QRect(60, 150, 281, 31));

        retranslateUi(ValidatorModal);

        QMetaObject::connectSlotsByName(ValidatorModal);
    } // setupUi

    void retranslateUi(QWidget *ValidatorModal)
    {
        ValidatorModal->setWindowTitle(QCoreApplication::translate("ValidatorModal", "Form", nullptr));
        TitleValidator->setText(QCoreApplication::translate("ValidatorModal", "<html><head/><body><p align=\"center\"><span style=\" font-size:14pt; color:#000000;\">Validador</span></p></body></html>", nullptr));
        TextValidator->setText(QCoreApplication::translate("ValidatorModal", "<html><head/><body><p align=\"center\"><span style=\" font-size:8pt;\"><br/></span></p></body></html>", nullptr));
        labelText->setText(QCoreApplication::translate("ValidatorModal", "<html><head/><body><p align=\"center\"><span style=\" font-size:9pt; color:#000000;\">Ingrese el codigo de la casa</span></p></body></html>", nullptr));
        ButtonValidator->setText(QCoreApplication::translate("ValidatorModal", "Verificar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ValidatorModal: public Ui_ValidatorModal {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VALIDATORMODAL_H
