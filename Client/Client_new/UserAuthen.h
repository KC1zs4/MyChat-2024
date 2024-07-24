#ifndef USERAUTHEN_H
#define USERAUTHEN_H

#include "IUserAuthen.h"
#include "NetworkTcp.h"

class UserAuthen: public IUserAuthen
{
    Q_OBJECT

private:
    INetworkTcp* m_network_tcp;

public:
    UserAuthen(INetworkTcp* network_tcp, QObject* parent = nullptr);
    ~UserAuthen();

    // 公共接口
    bool loginUser(const QString& client_id, const QString& client_password, const QString& host, quint16 port);
    bool registerUser(const QString& client_id, const QString& client_password, const QString& host, quint16 port);

private:
    void errorOccurred(const QString& error);

private slots:
    void do_userAuthenAffair(const QJsonObject& json);
};

#endif // USERAUTHEN_H
