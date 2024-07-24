#ifndef ENUMDATABASE_H
#define ENUMDATABASE_H

#include <QString>

/* * * * * * 1. user_info表 * * * * * */

enum class DatabaseUserInfo{

    ClientId,
    ClientPassword,
    ClientNickname,
    ClientEmail,
    ClientSignature,
    IsOnline

};

enum class DatabaseFriendList{
    ClientId,
    FriendId,
    LastMsgTime,
    UnreadMsg,
    IsOpen
};

enum class DatabaseMessages{
    SenderId,
    ReceiverId,
    Content,
    Time
};

enum class DatabaseTableName{
    UserInfo,
    FriendList,
    Messages
};


QString toString(DatabaseUserInfo info);
QString toString(DatabaseFriendList friend_list);
QString toString(DatabaseMessages messages);
QString toString(DatabaseTableName table_name);

#endif // ENUMDATABASE_H
