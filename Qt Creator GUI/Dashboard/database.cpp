#include "database.h"



database::database(QString Host, QString User, QString Type, int Port, QString Password, QString Schema):
    m_host{Host},
    m_user{User},
    m_type{Type},
    m_port{Port},
    m_password{Password},
    m_schema{Schema},
    m_database{QSqlDatabase::addDatabase(Type)},
    m_query{m_database}
{
    this->m_database.setHostName(m_host);
    this->m_database.setPort(m_port);
    this->m_database.setUserName(m_user);
    this->m_database.setPassword(m_password);
    this->m_database.setDatabaseName(m_schema);
    this->ok = this->m_database.open();
    if(!this->ok){
        qDebug() << "Error al conectar";
        QMessageBox::critical(nullptr, "Error", this->m_database.lastError().text());
        return;
    }

}

int database::GetDato(QString NameDat)
{
    if(!this->GetConnection()){
        qDebug() << "Conexion fallida:" << this->m_query.lastError().text();
        return 0;
    }

    this->m_query.prepare("SELECT d.value as valor "
                          "from data d join data_type dt on dt.id = d.data_type_id "
                          "where dt.type = :NameDat "
                          "order by d.id DESC limit 1");

    this->m_query.bindValue(":NameDat", NameDat);

    if (this->m_query.lastError().isValid()) {
        qDebug() << "Error en la preparación de la consulta:" << this->m_query.lastError().text();
        return 0; // O manejar el error de alguna manera
    }

    if (this->m_query.exec() && this->GetConnection()) {
        if (this->m_query.next()) {
            this->m_valor = this->m_query.value(0).toString();
            return this->m_valor.toInt();
        }
    }
    return 0;
}

void database::PushDato(QString NameDat, int Value)
{
    if(!this->GetConnection()){
        qDebug() << "Conexion fallida:" << this->m_query.lastError().text();
        return;
    }

    this->m_query.prepare("SELECT id from `data_type` where `type` = :NameDat");
    this->m_query.bindValue(":NameDat", NameDat);
    if (this->m_query.lastError().isValid()) {
        qDebug() << "Error en la preparación de la consulta:" << this->m_query.lastError().text();
        return; // O manejar el error de alguna manera
    }
    if (this->m_query.exec()) {
        if (!this->m_query.next()) {
            // No se registro ningun dato
            this->m_query.prepare("INSERT INTO `brb0vzvtpbxxloikqzji`.`data_type` (type) VALUES (:NameDat)");
            this->m_query.bindValue(":NameDat", NameDat);
            this->m_query.exec();
        }
    }

    this->m_query.prepare("INSERT INTO `brb0vzvtpbxxloikqzji`.`data` (data_type_id, value, creation_date)"
                          "VALUES ("
                          "(SELECT id FROM `data_type` dt where dt.type = :NameDat),"
                          ":value,"
                          "DATE_DUB(NOW(), INTERVAL 3 HOUR))");
    this->m_query.bindValue(":NameDat", NameDat);
    this->m_query.bindValue(":value", Value);
    this->m_query.exec();
}


bool database::GetConnection()
{
    return this->ok;
}

bool database::LoginUser(QString Username, QString Password)
{
    if(!this->GetConnection()){
        qDebug() << "Conexion fallida:" << this->m_query.lastError().text();
        return 0;
    }

    this->m_query.prepare("SELECT id "
                          "from user "
                          "where username = :Username "
                          "and password = :Password "
                          "and remove_date is null "
                          "limit 1");
    this->m_query.bindValue(":Username", Username);
    this->m_query.bindValue(":Password", Password);
    if (this->m_query.lastError().isValid()) {
        qDebug() << "Error en la preparación de la consulta:" << this->m_query.lastError().text();
        return 0; // O manejar el error de alguna manera
    }
    if (this->m_query.exec()) {
        if (this->m_query.next()) {
            return 1;
        }else{
            return 0;
        }
    }
    return 0;
}

bool database::CreateUser(QString Username, QString Password)
{
    if(!this->GetConnection()){
        qDebug() << "Conexion fallida:" << this->m_query.lastError().text();
        return 0;
    }


    if (this->VerificateUser(Username)) {
        return 0;
    }else{
        this->m_query.prepare("INSERT INTO `brb0vzvtpbxxloikqzji`.`user` (username, password) VALUES (:Username, :Password)");
        this->m_query.bindValue(":Username", Username);
        this->m_query.bindValue(":Password", Password);
        this->m_query.exec();
        return 1;
    }

}

bool database::RemoveUser(QString Username)
{
    if(!this->GetConnection()){
        qDebug() << "Conexion fallida:" << this->m_query.lastError().text();
        return 0;
    }

    if (this->VerificateUser(Username)) {
        this->m_valor = this->m_query.value(0).toString();
        this->m_query.prepare("UPDATE user SET remove_date = DATE_DUB(NOW(), INTERVAL 3 HOUR)) WHERE id = :id");
        this->m_query.bindValue(":id", this->m_valor);
        if (this->m_query.lastError().isValid()) {
            qDebug() << "Error en la preparación de la consulta:" << this->m_query.lastError().text();
            return 0; // O manejar el error de alguna manera
        }
        if (this->m_query.exec()) {
            return 1;
        }
    }else{
        return 0;
    }
    return 0;
}

bool database::VerificateUser(QString NameDat)
{
    if(!this->GetConnection()){
        qDebug() << "Conexion fallida:" << this->m_query.lastError().text();
        return 0;
    }

    this->m_query.prepare("SELECT id from user where username = :Username and remove_date is null");
    this->m_query.bindValue(":Username", NameDat);
    if (this->m_query.lastError().isValid()) {
        qDebug() << "Error en la preparación de la consulta:" << this->m_query.lastError().text();
        return 0; // O manejar el error de alguna manera
    }

    if (this->m_query.exec()) {
        if (this->m_query.next()) {
            return 1;
        }else{
            return 0;
        }
    }else{
        return 0;
    }
}

int database::GetTime()
{
    if(!this->GetConnection()){
        qDebug() << "Conexion fallida:" << this->m_query.lastError().text();
        return 0;
    }

    this->m_query.prepare("SELECT UNIX_TIMESTAMP(NOW()) AS Fecha_Actual");

    if (this->m_query.lastError().isValid()) {
        qDebug() << "Error en la preparación de la consulta:" << this->m_query.lastError().text();
        return 0; // O manejar el error de alguna manera
    }

    if (this->m_query.exec() && this->GetConnection()) {
        if (this->m_query.next()) {
            this->m_valor = this->m_query.value(0).toString();
            return this->m_valor.toInt();
        }
    }
    return 0;
}

database::~database()
{
    this->m_database.close();
}


