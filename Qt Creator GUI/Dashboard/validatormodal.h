#ifndef VALIDATORMODAL_H
#define VALIDATORMODAL_H

#include <QDialog>
#include "programconfig.h"



namespace Ui {
class ValidatorModal;
}

class ValidatorModal : public QDialog
{
    Q_OBJECT

public:
    explicit ValidatorModal(QWidget *parent = nullptr,  QString User = "", QString Password = "");
    ~ValidatorModal();

private slots:
    void on_ButtonValidator_clicked();

private:
    Ui::ValidatorModal *ui;
    QString m_user;
    QString m_password;
};

#endif // VALIDATORMODAL_H
