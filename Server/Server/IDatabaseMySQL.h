#ifndef IDATABASEMYSQL_H
#define IDATABASEMYSQL_H

#include <QString>
#include <QMap>
#include <QList>
#include <QVariant>
#include <QObject>
#include "EnumDatabase.h"

class IDatabaseMySQL: public QObject{

    Q_OBJECT

public:
    IDatabaseMySQL(QObject* parent = nullptr):QObject(parent){}
    virtual ~IDatabaseMySQL(){}
    virtual bool insertRecord(const QString& table_name,const QMap<QString,QVariant>& data) = 0;
    virtual bool updateRecord(const QString& table_name,const QString& condition, const QMap<QString,QVariant>& data) = 0;
    virtual bool deleteRecord(const QString& table_name,const QString& condition) = 0;
    virtual QMap<QString, QVariant> selectRecord(const QString& table_name,const QString& condition) const = 0;    // 查语句，返回一句
    virtual QList<QMap<QString,QVariant>> selectRecords(const QString& table_name,const QString& condition) const = 0; //查语句，返回多行

};

#endif // IDATABASEMYSQL_H
