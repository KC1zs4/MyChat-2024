#ifndef FRIEND_H
#define FRIEND_H

class Friend{

public:

    QString friend_id;
    QString friend_nickname;
    QString friend_last_msg_time;
    int friend_unread_msg;
    bool friend_is_online;

    // JsonFriend可以直接从这个序列化而来

public:
    // 1. 构造函数
    Friend(const QString& id = "", const QString& nickname = "", const QString& last_msg_time = "", bool is_online = false, int unread_msg = 0)
        : friend_id(id),friend_nickname(nickname),friend_last_msg_time(last_msg_time),friend_unread_msg(unread_msg),friend_is_online(is_online){}

    // 2. 可以实现反序列化
    static Friend fromJson(const QJsonObject& obj){
        const QString& friend_id = obj[toString(JsonFriend::FriendId)].toString();
        const QString& friend_nickname = obj[toString(JsonFriend::FriendNickname)].toString();
        const QString& friend_last_msg_time = obj[toString(JsonFriend::LastMsgTime)].toString();
        bool friend_is_online = obj[toString(JsonFriend::IsOnline)].toBool();
        int friend_unread_msg = obj[toString(JsonFriend::UnreadMsg)].toInt();
        return Friend(friend_id,friend_nickname,friend_last_msg_time,friend_is_online,friend_unread_msg);
    }


};


#endif // FRIEND_H
