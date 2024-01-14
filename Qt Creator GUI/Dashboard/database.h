#ifndef DATABASE_H
#define DATABASE_H
#include <QtSql/QSqlError>
#include <QtSql/QSqlDatabase>
#include <QMessageBox>
#include <QtSql/QSqlQuery>
#include <QString>
#include <QDebug>



class database
{
public:
    database(QString Host, QString User, QString Type, int Port, QString Password, QString Schema);
    int GetDato(QString NameDat);
    void PushDato(QString NameDat, int Value);
    bool GetConnection(void);
    bool LoginUser(QString Username, QString Password);
    bool CreateUser(QString Username, QString Password);
    bool RemoveUser(QString Username);
    bool VerificateUser(QString NameDat);
    ~database();
private:
    QString m_host;
    QString m_user;
    QString m_type;
    int m_port;
    QString m_password;
    QString m_schema;
    QString m_valor;
    bool ok;
    QSqlDatabase m_database;
    QSqlQuery m_query;
};

#endif // DATABASE_H
