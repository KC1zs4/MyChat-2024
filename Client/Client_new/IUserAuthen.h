#ifndef IUSERAUTHEN_H
#define IUSERAUTHEN_H

#include <QString>
#include <QJsonObject>
#include <QTcpSocket>
#include <QAbstractSocket>
#include "NetworkTcp.h"

class IUserAuthen: public QObject{

    Q_OBJECT

public:
    IUserAuthen(QObject* parent = nullptr):QObject(parent){}
    virtual ~IUserAuthen(){}
    virtual bool loginUser(const QString& client_id, const QString& client_password, const QString& host, quint16 port) = 0;
    virtual bool registerUser(const QString& client_id, const QString& client_password, const QString& host, quint16 port) = 0;

signals:

    void loginAffair(const QJsonObject& json, INetworkTcp* network_tcp);
    void registerAffair(const QJsonObject& json);

};

#endif // IUSERAUTHEN_H
