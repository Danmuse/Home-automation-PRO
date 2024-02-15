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

    void on_LuzLevel();

    void on_checkBox_stateChanged(int arg1);

    void on_PauseMusic_clicked();

    void on_VolumeMusic();

    void on_PlayMusic_clicked();

    void on_ComboMusic_currentIndexChanged(int index);

    void on_Red_clicked();

    void on_White_clicked();

    void on_Blue_clicked();

    void on_Green_clicked();

private:
    Ui::Dialog *ui;
    QTimer *miTemporizador;
    SerialParams Information;
    bool Reproduciendo = false;
};

#endif // DIALOG_H
