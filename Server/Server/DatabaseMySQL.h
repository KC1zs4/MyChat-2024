#ifndef DATABASEMYSQL_H
#define DATABASEMYSQL_H

#include "IDatabaseMySQL.h"
#include <QSqlDatabase>
#include <QObject>

class DatabaseMySQL : public IDatabaseMySQL{

    Q_OBJECT

private:
    QSqlDatabase* m_db;
    QString m_connection;

public:
    explicit DatabaseMySQL(const QString& host,quint16 port, const QString& id, const QString& password, const QString& dbname,QObject* parent=nullptr);
    ~DatabaseMySQL();

    // 接口
    bool insertRecord(const QString& table_name,const QMap<QString,QVariant>& data) override;
    bool updateRecord(const QString& table_name,const QString& condition, const QMap<QString,QVariant>& data) override;
    bool deleteRecord(const QString& table_name,const QString& condition) override;
    QMap<QString, QVariant> selectRecord(const QString& table_name,const QString& condition) const override;    // 查语句，返回一句
    QList<QMap<QString,QVariant>> selectRecords(const QString& table_name,const QString& condition) const override; //查语句，返回多行

private:
    void errorOccurred(const QString &error) const;

};

#endif // DATABASEMYSQL_H
