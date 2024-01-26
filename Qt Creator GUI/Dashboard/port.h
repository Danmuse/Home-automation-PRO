#ifndef PORT_H
#define PORT_H
#include <QString>
#include <QDebug>
#include <QSerialPort>
#include <QList>
#include <QSerialPortInfo>
#include <QObject>

struct SerialParams{
    QString Param;
    QString Info;
};

class port : public QObject
{
public:
    port(QString Puerto);
    port();
    ~port();
    QList<QSerialPortInfo> GetTotalsPorts();
    void SetPort(QString PortNew);
    bool OpenPort();
    void ClosePort();
    bool GetPortStatus();
    bool SendData(QString Data);
    SerialParams GetDato();
private:
    QSerialPort* m_Puerto;
    QList<QSerialPortInfo> m_Totals_Ports;
    QString m_PortName;
    QByteArray m_Dato;
    bool m_Flag;

private slots:
    void onReadyRead();
};

#endif // PORT_H
