#ifndef NETWORKTCP_H
#define NETWORKTCP_H

#include "INetworkTcp.h"

class NetworkTcp: public INetworkTcp{
    Q_OBJECT

private:
    QTcpSocket* m_tcp_socket;

public:
    explicit NetworkTcp(QObject* parent = nullptr);
    ~NetworkTcp();

    // 接口实现
    bool connectToHost(const QString& host, quint16 port);
    bool disconnectFromHost();
    bool send(const QJsonObject& json);
    QAbstractSocket::SocketState getSocketState();

private:
    void errorOccurred(const QString& error);


private slots:
    void do_disconnected();
    void do_readyRead();
};

#endif // NETWORKTCP_H
