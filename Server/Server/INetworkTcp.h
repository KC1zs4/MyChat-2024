#ifndef INETWORKTCP_H
#define INETWORKTCP_H

#include <QObject>
#include <QJsonObject>
#include <QTcpSocket>
#include <QJsonParseError>
#include <EnumJson.h>

class INetworkTcp : public QObject
{
    Q_OBJECT
public:
    explicit INetworkTcp(QObject *parent = nullptr): QObject(parent){}
    virtual ~INetworkTcp(){}

    // 接口函数
    virtual bool send(const QJsonObject& json) const = 0;

signals:
    void received(const QJsonObject& json);
    void disconnectNetworkTcp();
};

#endif // INETWORKTCP_H
