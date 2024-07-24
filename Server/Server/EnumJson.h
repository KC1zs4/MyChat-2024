#ifndef ENUMJSON_H
#define ENUMJSON_H

#include <QString>
#include <QJsonObject>

/*
 * 1. Enum设计
 *      1. 使用的所有键值都可以在这里找到，主要用于标识键
 *      2. Type和Action和Info两个部分需要指定值，键对应的值都需要使用其他变量而不是Enum中的
 *          1. 其中info是用来组成用户信息的QJsonObject
 *      3. 添加一个friend，使用QJsonObject
 *      4. 再添加一个friends，使用QJsonArray来作为值
 * 2. 综上1.
 *      1. 对于Action、Type，使用JsonAction、Type中限定的类型来直接作为值
 *      2. 对于Friend、Info对应的值是一个QJsonObject，使用JsonFriend、JsonInfo作为对象键值
 *      3. 对于Friends对应QJsonArray，直接使用QJsonArray作为值即可
 *      4. 对于消息来说要如何处理？直接作为键值还是使用QJsonObject进行封装
 *          1. 分装层次性更好，但是有开销
 *          2. 不分装JsonKey中太多了
 *          3. 其他方法——使用一个新的类？也可以作为键值？
 */



/* * * * * * 1. JsonKey * * * * * */

enum class JsonKey{
    Type,   // 值QString，对应字段为JsonType类
    Action, // 值QString，对应字段为JsonAction类

    // 1. UserAuthen对应
    Register,   // 值QJsonObject 对应字段为JsonRegister类
    Login,      // 值QJsonObject，对应字段为JsonLogin类

    // 2. FriendMgr对应
    FriendListLoad, // 值QJsonObject，对应字段为JsonFriendListLoad类
    FriendListAdd,   // 值QJsonObject，对应字段为JsonFreindListAdd类
    FriendListDetete,    // 值QJsonObject，对应字段为JosonFriendListDelete类
    FriendListUpdate,   // 值QJsonObject，对应字段为JsonFriendListUpdate类
                        // 好友新登录时就发送这个信息，通知进行改变
    FriendListSetting,  // 值QJsonObject，对应字段为JsonFriendListSetting

    // 3. SettingWindow对应
    SettingSave,    // 值QJsonObject，对应字段为JsonSettingSave类
    SettingLogout,  // 值QJsonObject，对应字段为JsonSettingLogout类

    // 3. ChatMgr对应
    ChatLoad,   // 值QJsonObject，对应字段为JsonChatLoad
    ChatForward, // 值QJsonObject，对应字段为JsonChatForward
    ChatUpdate,  // 值QJsonObject，对应字段为QJsonChatUpdate
    ChatClose   // 值QJsonObject，对应字段为JsonChatClose

};






/* * * * * * 2. JsonType * * * * * */

enum class JsonType{
    UserAuthen,
    Chat,
    Friend,
    Setting
};







/* * * * * * 3. JsonAction * * * * * */

enum class JsonAction{
    Load,
    Add,
    Delete,
    Update,
    Forward,
    Login,
    Logout,
    Save,
    Register,
    Close
};







/* * * * * * 4. JsonInfo * * * * * */

enum class JsonInfo{

    // user_info 表
    ClientId,
    ClientNickname,
    ClientEmail,
    ClientSignature,

};







/* * * * * * 5. JsonRegister * * * * * */

enum class JsonRegister{

    ClientId,
    ClientPassword,
    Exist,
    Error,
    Success
};






/* * * * * * 6. JsonLogin * * * * * */

enum class JsonLogin{

    ClientId,
    ClientPassword,
    Exist,
    IsOnline,
    Error,
    Success,
    Info   // 值QJsonObject，对应字段为JsonInfo类
};







/* * * * * * 7. JsonFriend * * * * * */

enum class JsonFriend{

    FriendId,
    FriendNickname,
    LastMsgTime,
    UnreadMsg,
    IsOnline
};


/* * * * * * 8. JsonFriendListLoad * * * * * */

enum class JsonFriendListLoad{

    ClientId,
    FriendId,
    Friends // 值为QJsonArray
};


/* * * * * * 9. JsonFriendListAdd * * * * * */

enum class JsonFriendListAdd{

    ClientId,
    ClientNickname,
    FriendId,
    Success,
    Exist,
    LastMsgTime,
    Friend  // 值为QJsonObject

};

/* * * * * * 10. JsonFriendListDelete * * * * * */

enum class JsonFriendListDelete{

    ClientId,
    FriendId,
    Success
};


/* * * * * * 11. JsonSettingSave * * * * * */

enum class JsonSettingSave{

    ClientId,
    ClientNickname,
    ClientEmail,
    ClientSignature,
    Success
};


/* * * * * * 12. JsonSettingLogout * * * * * */

enum class JsonSettingLogout{

    ClientId,
    Success
};


/* * * * * * 13. JsonChatLoad * * * * * */

enum class JsonChatLoad{

    SenderId,
    ReceiverId,
    Dialogs,    // 值为QJsonArray，每一项为JsonDialog
    Info        // 值为QJsonObject
};

/* * * * * * 14. JsonDialog * * * * * */

enum class JsonDialog{

    DialogTime,
    DialogSenderId,
    DiaLogContent,

};

/* * * * * * 15. JsonChatForward * * * * * */

enum class JsonChatForward{

    SenderId,
    ReceiverId,
    Dialog  // 值是QJsonObject

};

/* * * * * * 16 JsonChatUpdate * * * * * */

enum class JsonChatUpdate{

    SenderId,
    LastMsgTime
};


/* * * * * * 17 JsonChatClose * * * * * */

enum class JsonChatClose{

    ClientId,
    FriendId,
};

/* * * * * * 18 JsonFriendListUpdate * * * * */

enum class JsonFriendListUpdate{

    IsOnline,
    FriendId
};

/* * * * * * 19 JsonFriendListSetting * * * * * */

enum class JsonFriendListSetting{

    ClientId,
    ClientNickname,
    ClientEmail,
    ClientSignature
};


QString toString(JsonKey key);
QString toString(JsonType type);
QString toString(JsonAction action);
QString toString(JsonInfo info);
QString toString(JsonRegister registr);
QString toString(JsonLogin login);
QString toString(JsonFriend fri);
QString toString(JsonFriendListLoad load);
QString toString(JsonFriendListAdd add);
QString toString(JsonFriendListDelete del);
QString toString(JsonSettingSave save);
QString toString(JsonSettingLogout logout);
QString toString(JsonChatLoad load);
QString toString(JsonDialog dialog);
QString toString(JsonChatForward forward);
QString toString(JsonChatUpdate update);
QString toString(JsonChatClose close);
QString toString(JsonFriendListUpdate update);
QString toString(JsonFriendListSetting setting);


#endif // ENUMJSON_H
