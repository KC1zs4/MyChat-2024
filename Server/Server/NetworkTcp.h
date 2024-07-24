#ifndef NETWORKTCP_H
#define NETWORKTCP_H

#include <INetworkTcp.h>

class NetworkTcp:public INetworkTcp
{
    Q_OBJECT

private:
    QTcpSocket* m_tcp_socket;

public:
    explicit NetworkTcp(QTcpSocket* tcp_socket, QObject* parent = nullptr);
    ~NetworkTcp();

    // 接口函数
    bool send(const QJsonObject& json) const override;

private:
    void errorOccurred(const QString& error) const;

private slots:

    // 接受信息
    void do_readyRead();
    void do_disconnected();


};

#endif // NETWORKTCP_H
