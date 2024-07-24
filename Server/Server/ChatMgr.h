#ifndef CHATMGR_H
#define CHATMGR_H


#include "IChatMgr.h"
#include "NetworkTcp.h"
#include "DatabaseMySQL.h"

class ChatMgr: public IChatMgr{

    Q_OBJECT

private:
    INetworkTcp* m_network_tcp;
    IDatabaseMySQL* m_db_mysql;

public:
    ChatMgr(INetworkTcp* network_tcp, IDatabaseMySQL* db_mysql, QObject* parent = nullptr);
    ~ChatMgr();

    // 接口函数
    bool handleChatLoad(const QJsonObject& json);
    bool handleChatForward(const QJsonObject& json);
    bool handleChatClose(const QJsonObject &json);

private:
    void errorOccurred(const QString& error);
};

#endif // CHATMGR_H
