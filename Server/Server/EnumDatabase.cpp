#include "EnumDatabase.h"

QString toString(DatabaseUserInfo info){
    switch(info){
    case DatabaseUserInfo::ClientId:  return "client_id";
    case DatabaseUserInfo::ClientPassword: return "client_password";
    case DatabaseUserInfo::ClientNickname:    return "client_nickname";
    case DatabaseUserInfo::ClientEmail:   return "client_email";
    case DatabaseUserInfo::ClientSignature:   return "client_signature";
    case DatabaseUserInfo::IsOnline: return "is_online";
    default:    return "unknown";
    }
}

QString toString(DatabaseFriendList friend_list){
    switch(friend_list){
    case DatabaseFriendList::ClientId:  return "client_id";
    case DatabaseFriendList::FriendId:  return "friend_id";
    case DatabaseFriendList::LastMsgTime:   return "last_msg_time";
    case DatabaseFriendList::UnreadMsg: return "unread_msg";
    case DatabaseFriendList::IsOpen:    return "is_open";
    default:    return "unknown";
    }
}

QString toString(DatabaseMessages messages){
    switch(messages){
    case DatabaseMessages::SenderId:    return "sender_id";
    case DatabaseMessages::ReceiverId:  return "receiver_id";
    case DatabaseMessages::Content: return "content";
    case DatabaseMessages::Time:    return "time";
    default:    return "unknown";
    }
}

QString toString(DatabaseTableName table_name){
    switch(table_name){
    case DatabaseTableName::UserInfo:   return "user_info";
    case DatabaseTableName::FriendList: return "friend_list";
    case DatabaseTableName::Messages:   return "messages";
    default:    return "unknown";
    }
}
