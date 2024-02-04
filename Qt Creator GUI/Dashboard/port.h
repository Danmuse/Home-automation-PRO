#ifndef PORT_H
#define PORT_H
#include <QString>
#include <QDebug>
#include <QSerialPort>
#include <QList>
#include <QSerialPortInfo>
#include <QObject>

#define HEADER '$'
#define FOOTER '%'


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
    QString m_Dato[4];
    int m_IndexRead = 0;
    int m_IndexWrite = 0;
    bool m_Flag;

private slots:
    void onReadyRead();
};

#endif // PORT_H
