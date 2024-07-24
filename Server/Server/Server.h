#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QMap>
#include <iostream>

#include "SingleSession.h"
#include "DatabaseMySQL.h"
#include "NetworkTcp.h"

class Server : public QObject
{
    Q_OBJECT

private:
    QTcpServer* m_tcp_server;
    QMap<QString,SingleSession*> m_single_sessions_online; // 通过id来寻找，并查找是否在线
    QMap<INetworkTcp*,SingleSession*> m_single_sessions;    // 每一个客户端对应一个session

    quint16 m_running_port;
    quint16 m_set_port;

    IDatabaseMySQL* m_db_mysql; // 主要用户查看用户是否在线

public:
    explicit Server(QObject *parent = nullptr);
    ~Server();

    // 接口函数
    bool startServer();
    bool stopServer();
    bool setPort();
    void showStatus() const;

private:

    // 辅助函数
    bool handleRegister(const QJsonObject& json, INetworkTcp* network_tcp);
    bool handleLogin(const QJsonObject& json, INetworkTcp* network_tcp);
    // Setting相关的
    bool handleSettingSave(const QJsonObject& json, INetworkTcp* network_tcp);
    bool handleSettingLogout(const QJsonObject& json, INetworkTcp* network_tcp);
    // ChatWindow相关的
    bool handleChatForwardOff(const QJsonObject& json);
    // FriendListWindow相关的
    bool handleFriendListUpdate(const QString& client_id, bool is_online);
    bool handleFriendListSetting(const QString& client_id, const QString& client_nickname, const QString& client_email, const QString& client_signature);
    void errorOccurred(const QString& error) const;

signals:

private slots:
    void do_newConnection();

    // 处理NetworkTcp中的信号
    void do_disconnectedNetworkTcp();
    void do_received(const QJsonObject& json);

    // 处理SingleSession中的信号
    void do_newFriendSession(const QJsonObject& json, const QString& friend_id);
    void do_delFriendSession(const QJsonObject& json, const QString& friend_id);
};

#endif // SERVER_H
