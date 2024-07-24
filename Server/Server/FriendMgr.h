#ifndef FRIENDMGR_H
#define FRIENDMGR_H

#include "IFriendMgr.h"
#include "DatabaseMySQL.h"
#include "NetworkTcp.h"

class FriendMgr: public IFriendMgr{

    Q_OBJECT

private:
    IDatabaseMySQL* m_db_mysql;
    INetworkTcp* m_network_tcp;

public:
    FriendMgr(IDatabaseMySQL* db_mysql, INetworkTcp* network_tcp ,QObject* parent = nullptr);
    ~FriendMgr();

    // 接口函数
    bool handleFriendListAdd(const QJsonObject& json);
    bool handleFriendListDelete(const QJsonObject& json);
    bool handleFriendListLoad(const QJsonObject& json);
    bool handleFriendListUpdate(const QJsonObject &json);
    bool handleFriendListSetting(const QJsonObject& json);


    bool handleNewFriendComing(const QJsonObject& json);
    bool handleDelFriendComing(const QJsonObject &json);

private:

    // 错误处理函数
    void errorOccurred(const QString& error);
};

#endif // FRIENDMGR_H
