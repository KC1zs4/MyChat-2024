#ifndef USERAUTHEN_H
#define USERAUTHEN_H

#include "IUserAuthen.h"
#include "IDatabaseMySQL.h"
#include "INetworkTcp.h"

class UserAuthen: public IUserAuthen
{
    Q_OBJECT

private:
    IDatabaseMySQL* m_db_mysql;
    INetworkTcp* m_network_tcp;

public:
    UserAuthen(IDatabaseMySQL* db_mysql, INetworkTcp* network_tcp, QObject* parent = nullptr);
    ~UserAuthen() override;

    // 接口函数
    bool RegisterUser(const QString& client_id, const QString& client_password) override;
    bool loginUser(const QString& client_id, const QString& client_password)override;
    bool logoutUser(const QString& client_id) override;

private:
    void errorOccurred(const QString& error) const;
};

#endif // USERAUTHEN_H
