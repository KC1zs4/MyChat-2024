#ifndef MESSAGE_H
#define MESSAGE_H

#include <QJsonObject>
#include <QDateTime>
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
    explicit RegisterMessage(const QString& client_id, const QString& client_password){
        m_json_object[toString(JsonKey::Type)] = toString(JsonType::UserAuthen);
        m_json_object[toString(JsonKey::Action)] = toString(JsonAction::Register);
        QJsonObject registr;
        registr[toString(JsonRegister::ClientId)] = client_id;
        registr[toString(JsonRegister::ClientPassword)] = client_password;
        m_json_object[toString(JsonKey::Register)] = registr;
    }

    ~RegisterMessage() override{}
};

class LoginMessage: public Message{
public:
    explicit LoginMessage(const QString& client_id, const QString& client_password){
        m_json_object[toString(JsonKey::Type)] = toString(JsonType::UserAuthen);
        m_json_object[toString(JsonKey::Action)] = toString(JsonAction::Login);

        QJsonObject login;
        login[toString(JsonLogin::ClientId)] = client_id;
        login[toString(JsonLogin::ClientPassword)] = client_password;
        m_json_object[toString(JsonKey::Login)] = login;
    }

    ~LoginMessage(){}
};

class FriendListLoadMessage: public Message{
public:
    explicit FriendListLoadMessage(const QString& client_id){

        m_json_object[toString(JsonKey::Type)] = toString(JsonType::Friend);
        m_json_object[toString(JsonKey::Action)] = toString(JsonAction::Load);

        QJsonObject friend_list_load;
        friend_list_load[toString(JsonFriendListLoad::ClientId)] = client_id;
        m_json_object[toString(JsonKey::FriendListLoad)] = friend_list_load;
    }
};

class FriendListAddMessage: public Message{
public:
    explicit FriendListAddMessage(const QString& client_id, const QString& client_nickname, const QString& friend_id){
        m_json_object[toString(JsonKey::Type)] = toString(JsonType::Friend);
        m_json_object[toString(JsonKey::Action)] = toString(JsonAction::Add);
        QJsonObject add;
        add[toString(JsonFriendListAdd::ClientId)] = client_id;
        add[toString(JsonFriendListAdd::ClientNickname)] = client_nickname;
        add[toString(JsonFriendListAdd::FriendId)] = friend_id;
        add[toString(JsonFriendListAdd::LastMsgTime)] = QDateTime::currentDateTime().toString(Qt::ISODate);
        m_json_object[toString(JsonKey::FriendListAdd)] = add;
    }
};

class FriendListDeleteMessage: public Message{
public:
    explicit FriendListDeleteMessage(const QString& client_id, const QString& friend_id){
        m_json_object[toString(JsonKey::Type)] = toString(JsonType::Friend);
        m_json_object[toString(JsonKey::Action)] = toString(JsonAction::Delete);
        QJsonObject del;
        del[toString(JsonFriendListDelete::ClientId)] = client_id;
        del[toString(JsonFriendListDelete::FriendId)] = friend_id;
        m_json_object[toString(JsonKey::FriendListDetete)] = del;
    }
};

class SettingSaveMessage: public Message{
public:
    explicit SettingSaveMessage(const QString& client_id,const QString& client_nickname,const QString& client_email,const QString& client_signature){
        m_json_object[toString(JsonKey::Type)] = toString(JsonType::Setting);
        m_json_object[toString(JsonKey::Action)] = toString(JsonAction::Save);
        QJsonObject save;
        save[toString(JsonSettingSave::ClientId)] = client_id;
        save[toString(JsonSettingSave::ClientNickname)] = client_nickname;
        save[toString(JsonSettingSave::ClientEmail)] = client_email;
        save[toString(JsonSettingSave::ClientSignature)] = client_signature;
        m_json_object[toString(JsonKey::SettingSave)] = save;
    }
};

class SettingLogoutMessage: public Message{
public:
    explicit SettingLogoutMessage(const QString& client_id){
        m_json_object[toString(JsonKey::Type)] = toString(JsonType::Setting);
        m_json_object[toString(JsonKey::Action)] = toString(JsonAction::Logout);
        QJsonObject logout;
        logout[toString(JsonSettingLogout::ClientId)] = client_id;
        m_json_object[toString(JsonKey::SettingLogout)] = logout;
    }
};

class ChatLoadMessage: public Message{

public:
    explicit ChatLoadMessage(const QString& sender_id, const QString& receiver_id){
        m_json_object[toString(JsonKey::Type)] = toString(JsonType::Chat);
        m_json_object[toString(JsonKey::Action)] = toString(JsonAction::Load);
        QJsonObject load;
        load[toString(JsonChatLoad::SenderId)] = sender_id;
        load[toString(JsonChatLoad::ReceiverId)] = receiver_id;
        m_json_object[toString(JsonKey::ChatLoad)] = load;
    }
};

class ChatForwardMessage: public Message{
public:
    explicit ChatForwardMessage(const QString& sender_id, const QString& receiver_id, const QJsonObject& dialog){
        m_json_object[toString(JsonKey::Type)] = toString(JsonType::Chat);
        m_json_object[toString(JsonKey::Action)] = toString(JsonAction::Forward);
        QJsonObject forward;
        forward[toString(JsonChatForward::SenderId)] = sender_id;
        forward[toString(JsonChatForward::ReceiverId)] = receiver_id;
        forward[toString(JsonChatForward::Dialog)] = dialog;
        m_json_object[toString(JsonKey::ChatForward)] = forward;
    }
};

class ChatCloseMessage: public Message{
public:
    explicit ChatCloseMessage(const QString& client_id, const QString& friend_id){
        m_json_object[toString(JsonKey::Type)] = toString(JsonType::Chat);
        m_json_object[toString(JsonKey::Action)] = toString(JsonAction::Close);
        QJsonObject close;
        close[toString(JsonChatClose::ClientId)] = client_id;
        close[toString(JsonChatClose::FriendId)] = friend_id;
        m_json_object[toString(JsonKey::ChatClose)] = close;
    }
};

#endif // MESSAGE_H
