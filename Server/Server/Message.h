#ifndef MESSAGE_H
#define MESSAGE_H

#include <QJsonObject>
#include <QJsonArray>
#include "EnumJson.h"

class Message{
protected:
    QJsonObject m_json_object;

public:

    explicit Message(){}
    virtual ~Message(){}

    QJsonObject getJsonObejct(){
        return m_json_object;
    }

};

class RegisterMessage: public Message{

public:
    explicit RegisterMessage(bool success, bool exist, bool error){
        m_json_object[toString(JsonKey::Type)] = toString(JsonType::UserAuthen);
        m_json_object[toString(JsonKey::Action)] = toString(JsonAction::Register);
        QJsonObject registr;    // 将register返回的信息包装在一个QJsonObject中，直接提取出来就和以前一样了
        registr[toString(JsonRegister::Success)] = success;
        registr[toString(JsonRegister::Error)] = error;
        registr[toString(JsonRegister::Exist)] = exist;
        m_json_object[toString(JsonKey::Register)] = registr;
    }

    ~RegisterMessage() override{}
};

class LoginMessage: public Message{
public:
    explicit LoginMessage(bool success, bool exist, bool is_online, bool error,const QJsonObject& json){
        m_json_object[toString(JsonKey::Type)] = toString(JsonType::UserAuthen);
        m_json_object[toString(JsonKey::Action)] = toString(JsonAction::Login);
        QJsonObject login;  // 将login返回的信息包装在一个QJsonObject中，直接取出来就和以前一样了，直接用
        login[toString(JsonLogin::Success)] = success;
        login[toString(JsonLogin::IsOnline)] = is_online;
        login[toString(JsonLogin::Exist)] = exist;
        login[toString(JsonLogin::Error)] = error;
        login[toString(JsonLogin::Info)] = json;
        m_json_object[toString(JsonKey::Login)] = login;
    }

    ~LoginMessage(){}
};

class FriendListLoadMessage: public Message{
public:
    explicit FriendListLoadMessage(const QJsonArray& friends_array){
        // 只需要把好友列表发送回去即可
        m_json_object[toString(JsonKey::Type)] = toString(JsonType::Friend);
        m_json_object[toString(JsonKey::Action)] = toString(JsonAction::Load);
        QJsonObject json;
        json[toString(JsonFriendListLoad::Friends)] = friends_array;
        m_json_object[toString(JsonKey::FriendListLoad)] = json;
    }

    ~FriendListLoadMessage(){}
};

class FriendListAddMessage: public Message{
public:
    explicit FriendListAddMessage(bool success, bool exist, const QJsonObject& fri){
        m_json_object[toString(JsonKey::Type)] = toString(JsonType::Friend);
        m_json_object[toString(JsonKey::Action)] = toString(JsonAction::Add);
        QJsonObject json;
        json[toString(JsonFriendListAdd::Success)] = success;
        json[toString(JsonFriendListAdd::Friend)] = fri;
        json[toString(JsonFriendListAdd::Exist)] = exist;
        m_json_object[toString(JsonKey::FriendListAdd)] = json;

    }
};

class FriendListDeleteMessage: public Message{
public:
    explicit FriendListDeleteMessage(bool success, const QString& fri_id){
        m_json_object[toString(JsonKey::Type)] = toString(JsonType::Friend);
        m_json_object[toString(JsonKey::Action)] = toString(JsonAction::Delete);
        QJsonObject json;
        json[toString(JsonFriendListDelete::FriendId)] = fri_id;
        json[toString(JsonFriendListDelete::Success)] = success;
        m_json_object[toString(JsonKey::FriendListDetete)] = json;
    }
};

class SettingSaveMessage: public Message{
public:
    explicit SettingSaveMessage(bool success, QJsonObject save){
        m_json_object[toString(JsonKey::Type)] = toString(JsonType::Setting);
        m_json_object[toString(JsonKey::Action)] = toString(JsonAction::Save);
        save[toString(JsonSettingSave::Success)] = success;
        m_json_object[toString(JsonKey::SettingSave)] = save;
    }
};

class SettingLogoutMessage: public Message{
public:
    explicit SettingLogoutMessage(bool success){
        m_json_object[toString(JsonKey::Type)] = toString(JsonType::Setting);
        m_json_object[toString(JsonKey::Action)] = toString(JsonAction::Logout);
        QJsonObject json;
        json[toString(JsonSettingLogout::Success)] = success;
        m_json_object[toString(JsonKey::SettingLogout)] = json;
    }
};

class ChatLoadMessage: public Message{
public:
    explicit ChatLoadMessage(const QString& friend_id, const QJsonObject& info,const QJsonArray& dialogs){
        m_json_object[toString(JsonKey::Type)] = toString(JsonType::Chat);
        m_json_object[toString(JsonKey::Action)] = toString(JsonAction::Load);
        QJsonObject load;
        load[toString(JsonChatLoad::SenderId)] = friend_id;
        load[toString(JsonChatLoad::Info)] = info;
        load[toString(JsonChatLoad::Dialogs)] = dialogs;
        m_json_object[toString(JsonKey::ChatLoad)] = load;
    }
};

class ChatUpdateMessage: public Message{
public:
    explicit ChatUpdateMessage(const QString& sender_id, const QString& last_msg_time){
        m_json_object[toString(JsonKey::Type)] = toString(JsonType::Chat);
        m_json_object[toString(JsonKey::Action)] = toString(JsonAction::Update);
        QJsonObject update;
        update[toString(JsonChatUpdate::SenderId)] = sender_id;
        update[toString(JsonChatUpdate::LastMsgTime)] = last_msg_time;
        m_json_object[toString(JsonKey::ChatUpdate)] = update;

    }
};

class FriendListUpdateMessage: public Message{
public:
    explicit FriendListUpdateMessage(bool is_online,const QString& friend_id){
        m_json_object[toString(JsonKey::Type)] = toString(JsonType::Friend);
        m_json_object[toString(JsonKey::Action)] = toString(JsonAction::Update);
        QJsonObject update;
        update[toString(JsonFriendListUpdate::IsOnline)] = is_online;
        update[toString(JsonFriendListUpdate::FriendId)] = friend_id;
        m_json_object[toString(JsonKey::FriendListUpdate)] = update;
    }
};

class FriendListSettingMessage: public Message{
public:
    explicit FriendListSettingMessage(const QString& client_id, const QString& client_nickname, const QString& client_email, const QString& client_signature){
        m_json_object[toString(JsonKey::Type)] = toString(JsonType::Friend);
        m_json_object[toString(JsonKey::Action)] = toString(JsonAction::Save);
        QJsonObject setting;
        setting[toString(JsonFriendListSetting::ClientId)] = client_id;
        setting[toString(JsonFriendListSetting::ClientNickname)] = client_nickname;
        setting[toString(JsonFriendListSetting::ClientEmail)] = client_email;
        setting[toString(JsonFriendListSetting::ClientSignature)] = client_signature;
        m_json_object[toString(JsonKey::FriendListSetting)] = setting;
    }
};

#endif // MESSAGE_H
