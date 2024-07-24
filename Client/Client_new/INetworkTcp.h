#ifndef INETWORKTCP_H
#define INETWORKTCP_H

#include <QObject>
#include <QTcpSocket>
#include <QString>
#include <QJsonObject>
#include <QJsonDocument>
#include <QByteArray>
#include "message.h"
#include "EnumJson.h"

class INetworkTcp: public QObject{

    Q_OBJECT

public:
    INetworkTcp(QObject* parent = nullptr): QObject(parent){}
    virtual ~INetworkTcp(){}

    // 接口函数
    virtual bool connectToHost(const QString& host, quint16 port) = 0;
    virtual bool disconnectFromHost() = 0;
    virtual bool send(const QJsonObject& json) = 0;
    virtual QAbstractSocket::SocketState getSocketState() = 0;


signals:
    void disconnectedNetworkTcp();

    // 接受到的不同消息，作为转接
    void userAuthenAffair(const QJsonObject& json);
    void friendAffair(const QJsonObject& json);
    void chatAffair(const QJsonObject& json);
    void settingAffair(const QJsonObject& json);
};

#endif // INETWORKTCP_H
