#ifndef SINGLESESSION_H
#define SINGLESESSION_H

#include <QObject>
#include "UserAuthen.h"
#include "FriendMgr.h"
#include "ChatMgr.h"

class SingleSession : public QObject
{
    Q_OBJECT

private:
    QString m_client_id;
    IUserAuthen* m_user_authen;
    IFriendMgr* m_friend_mgr;
    IChatMgr* m_chat_mgr;

public:
    explicit SingleSession(const QString& client_id, INetworkTcp* network_tcp, IDatabaseMySQL* db_mysql, QObject *parent = nullptr);
    ~SingleSession();

    // 公共接口
    QString getId();

    // 1. 用户认证的接口
    bool loginSession(const QJsonObject& json);
    bool registerSession(const QJsonObject& json);  // 在内部进行消息的解析处理，server只负责分发信息
    bool logoutSession();

    // 2. 用户好友管理的接口
    bool FriendListLoad(const QJsonObject& json);
    bool FriendListAdd(const QJsonObject& json);
    bool FriendListDelete(const QJsonObject& json);
    bool FriendListUpdate(const QJsonObject& json);
    bool FriendListSetting(const QJsonObject& json);

    // 3. 别人添加为好友的消息发送
    bool newFriendComing(const QJsonObject& json);
    bool delFriendComing(const QJsonObject& json);

    // 4. 处理chat信息的
    bool chatLoad(const QJsonObject& json);
    bool chatForward(const QJsonObject& json);
    bool chatClose(const QJsonObject& json);

private:

    void errorOccurred(const QString& error);


private slots:

    void do_newFriendMgr(const QJsonObject& json,const QString& friend_id);
    void do_delFriendMgr(const QJsonObject& json,const QString& friend_id);

signals:

    void newFriendSession(const QJsonObject& json,const QString& friend_id);
    void delFriendSession(const QJsonObject& json,const QString& friend_id);
};

#endif // SINGLESESSION_H
