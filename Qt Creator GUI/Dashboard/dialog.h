#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "programconfig.h"
#include <QTimer>
#include <QObject>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

private slots:
    void on_pushButton_closeSession_clicked();
    void manejarTimeOut();

    void on_ToggleLuz_clicked();

    void on_LuzLevel_actionTriggered(int action);

private:
    Ui::Dialog *ui;
    QTimer *miTemporizador;
    SerialParams Information;
};

#endif // DIALOG_H
