#include "EnumJson.h"


QString toString(JsonKey key) {
    switch (key) {
    case JsonKey::Type:         return "type";
    case JsonKey::Action:       return "action";
    case JsonKey::Login:        return "login";
    case JsonKey::Register:     return "register";
    case JsonKey::FriendListLoad:   return "friend_list_load";
    default:                    return "unknown";
    }
}

QString toString(JsonType type) {
    switch (type) {
    case JsonType::UserAuthen: return "userauthen";
    case JsonType::Chat:       return "chat";
    case JsonType::Friend:     return "friend";
    case JsonType::Setting:     return "setting";
    default:                   return "unknown";
    }
}



QString toString(JsonAction action) {
    switch (action) {
    case JsonAction::Load:      return "load";
    case JsonAction::Add:       return "add";
    case JsonAction::Delete:    return "delete";
    case JsonAction::Update:    return "update";
    case JsonAction::Forward:   return "forward";
    case JsonAction::Login:     return "login";
    case JsonAction::Logout:    return "logout";
    case JsonAction::Register: return "register";
    case JsonAction::Save:      return "save";
    case JsonAction::Close:     return "close";
    default:                    return "unknown";
    }
}



QString toString(JsonInfo info){
    switch (info) {
    case JsonInfo::ClientId:        return "client_id";
    case JsonInfo::ClientNickname:   return "client_nickname";
    case JsonInfo::ClientEmail:      return "client_email";
    case JsonInfo::ClientSignature:  return "client_signature";
    default:                         return "unknown";
    }
}


QString toString(JsonRegister registr){
    switch(registr){
    case JsonRegister::ClientId:    return "client_id";
    case JsonRegister::ClientPassword:  return "client_password";
    case JsonRegister::Exist:       return "exist";
    case JsonRegister::Error:       return "error";
    case JsonRegister::Success:     return "success";
    default:    return "unknown";
    }
}

QString toString(JsonLogin login){
    switch(login){
    case JsonLogin::ClientId:    return "client_id";
    case JsonLogin::ClientPassword:  return "client_password";
    case JsonLogin::Exist:       return "exist";
    case JsonLogin::Error:       return "error";
    case JsonLogin::Success:     return "success";
    case JsonLogin::IsOnline:    return "is_online";
    case JsonLogin::Info:         return "info";
    default:    return "unknown";
    }
}


QString toString(JsonFriend fri){
    switch(fri){
    case JsonFriend::FriendId:  return "friend_id";
    case JsonFriend::LastMsgTime:   return "last_msg_time";
    case JsonFriend::UnreadMsg: return "unread_msg";
    case JsonFriend::IsOnline:    return "is_open";
    default:    return "unknown";
    }
}

QString toString(JsonFriendListLoad load){
    switch(load){
    case JsonFriendListLoad::ClientId:  return "client_id";
    case JsonFriendListLoad::FriendId:  return "friend_id";
    case JsonFriendListLoad::Friends:   return "friends";
    default:    return "unknown";
    }
}

QString toString(JsonFriendListAdd add){
    switch(add){
    case JsonFriendListAdd::ClientId:   return "client_id";
    case JsonFriendListAdd::ClientNickname:     return "client_nickname";
    case JsonFriendListAdd::FriendId:   return "friend_id";
    case JsonFriendListAdd::Success:    return "success";
    case JsonFriendListAdd::Exist:      return "exist";
    case JsonFriendListAdd::Friend:     return "friend";
    case JsonFriendListAdd::LastMsgTime:    return "last_msg_time";
    default:                            return "unknown";
    }
}

QString toString(JsonFriendListDelete del){
    switch(del){
    case JsonFriendListDelete::ClientId:    return "client_id";
    case JsonFriendListDelete::FriendId:    return "friend_id";
    case JsonFriendListDelete::Success:     return "success";
    default:                                return "unknown";
    }
}

QString toString(JsonSettingSave save){
    switch(save){
    case JsonSettingSave::ClientId: return "client_id";
    case JsonSettingSave::ClientEmail:  return "client_email";
    case JsonSettingSave::ClientNickname: return "client_nickname";
    case JsonSettingSave::ClientSignature: return "client_signature";
    case JsonSettingSave::Success:      return "success";
    default:                                return "unknown";
    }
}

QString toString(JsonSettingLogout logout){
    switch(logout){
    case JsonSettingLogout::ClientId: return "client_id";
    case JsonSettingLogout::Success:    return "success";
    default:                        return "unknown";
    }
}

QString toString(JsonChatLoad load){
    switch(load){
    case JsonChatLoad::SenderId:    return "sender_id";
    case JsonChatLoad::ReceiverId:  return "receiver_id";
    case JsonChatLoad::Dialogs:     return "dialogs";
    case JsonChatLoad::Info:      return "info";
    default:                        return "unknown";
    }
}

QString toString(JsonDialog dialog){
    switch(dialog){
    case JsonDialog::DialogTime:    return "dialog_time";
    case JsonDialog::DialogSenderId:    return "dialog_sender_id";
    case JsonDialog::DiaLogContent: return "dialog_content";
    default:                        return "unknown";
    }
}

QString toString(JsonChatForward forward){
    switch(forward){
    case JsonChatForward::SenderId: return "sender_id";
    case JsonChatForward::Dialog:   return "dialog";
    case JsonChatForward::ReceiverId: return "receiver_id";
    default:                    return "unknown";
    }
}

QString toString(JsonChatUpdate update){
    switch(update){
    case JsonChatUpdate::SenderId:  return "sender_id";
    case JsonChatUpdate::LastMsgTime:   return "last_msg_time";
    default:                        return "unknown";
    }
}

QString toString(JsonChatClose close){
    switch(close){
    case JsonChatClose::ClientId:   return "client_id";
    case JsonChatClose::FriendId:   return "friend_id";
    default:                        return "unknown";
    }
}

QString toString(JsonFriendListUpdate update){
    switch(update){
    case JsonFriendListUpdate::FriendId:    return "friend_id";
    case JsonFriendListUpdate::IsOnline:    return "is_online";
    default:                                return "unknown";
    }
}

QString toString(JsonFriendListSetting setting){
    switch(setting){
    case JsonFriendListSetting::ClientId:   return "client_id";
    case JsonFriendListSetting::ClientNickname: return "client_nickname";
    case JsonFriendListSetting::ClientEmail:    return "client_email";
    case JsonFriendListSetting::ClientSignature:    return "client_signature";
    default:                                return "unknown";
    }
}




