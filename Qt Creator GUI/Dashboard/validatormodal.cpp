#include "validatormodal.h"
#include "ui_validatormodal.h"
#include <QMessageBox>

ValidatorModal::ValidatorModal(QWidget *parent, QString User, QString Password)
    : QDialog(parent), m_user{User}, m_password{Password}
    , ui(new Ui::ValidatorModal)
{
    ui->setupUi(this);
}

ValidatorModal::~ValidatorModal()
{
    delete ui;
}

void ValidatorModal::on_ButtonValidator_clicked()
{
    if(Codigo == ui->CodeText->text()){
        if(Database.CreateUser(m_user, m_password)){
            QMessageBox::information(this, "Aviso", "Se creo el usuario correctamente" );
        }else{
            QMessageBox::warning(this, "Aviso", "No se pudo crear el usuario" );
        }
        this->close();
    }else{

        ui->TextValidator->setEnabled(true);
        ui->TextValidator->setStyleSheet(" color: #D8000C;"
                                              "font-size: 14px;"
                                              "font-weight: bold;"
                                              "background-color: #FFD2D2;"
                                              "border: 1px solid #B30000; "
                                              "border-radius: 5px; "
                                              "padding: 5px;");
        ui->TextValidator->setText("Codigo Invalido");
    }
}

