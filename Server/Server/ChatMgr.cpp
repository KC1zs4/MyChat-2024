#include "ChatMgr.h"

/* * * * * * * 1. 对象的初始化和销毁 * * * * * * */

/*
 * 1. 构造函数
 */
ChatMgr::ChatMgr(INetworkTcp *network_tcp, IDatabaseMySQL *db_mysql, QObject *parent)
    : IChatMgr(parent), m_network_tcp(network_tcp),m_db_mysql(db_mysql){}

/*
 * 2. 析构函数
 */
ChatMgr::~ChatMgr(){
    qDebug() << "~ChatMgr called";
}

/* * * * * * * 2. 接口函数实现 * * * * * * */

/*
 * 1. 加载聊天信息
 */
bool ChatMgr::handleChatLoad(const QJsonObject &json){

    // 1. 获取好友和用户id
    const QString& client_id = json[toString(JsonChatLoad::ReceiverId)].toString();
    const QString& friend_id = json[toString(JsonChatLoad::SenderId)].toString();

    // 2. 获取聊天记录，盆友发送，我接受
    QString condition = toString(DatabaseMessages::SenderId) + " = '" + friend_id + "' AND " + toString(DatabaseMessages::ReceiverId) + " = '" + client_id + "' ORDER BY time ASC";
    const QList<QMap<QString,QVariant>>& dialogs = m_db_mysql->selectRecords(toString(DatabaseTableName::Messages),condition);
    QJsonArray array;
    for(const QMap<QString,QVariant>& dialog_record: dialogs){
        QJsonObject dialog;
        dialog[toString(JsonDialog::DialogTime)] = dialog_record[toString(DatabaseMessages::Time)].toString();
        dialog[toString(JsonDialog::DiaLogContent)] = dialog_record[toString(DatabaseMessages::Content)].toString();
        // 可以添加sender_id，因为sender_id肯定是对方，可以在客户端进行处理
        array.append(dialog);
    }

    // 3. 获取好友信息
    condition = toString(DatabaseUserInfo::ClientId) + " = '" + friend_id + "'";
    QMap<QString,QVariant> friend_info = m_db_mysql->selectRecord(toString(DatabaseTableName::UserInfo),condition);
    QJsonObject info;
    info[toString(JsonInfo::ClientEmail)] = friend_info[toString(DatabaseUserInfo::ClientEmail)].toString();
    info[toString(JsonInfo::ClientSignature)] = friend_info[toString(DatabaseUserInfo::ClientSignature)].toString();

    // 4. 更改表中信息，加载意味着打开窗口，将unreadMsg设置为0并将isOpen设置为1
    condition = toString(DatabaseFriendList::ClientId) + " = '" + client_id + "' and " + toString(DatabaseFriendList::FriendId) + " = '" + friend_id + "'";
    QMap<QString,QVariant> update;
    update.insert(toString(DatabaseFriendList::IsOpen),1);
    update.insert(toString(DatabaseFriendList::UnreadMsg),0);
    m_db_mysql->updateRecord(toString(DatabaseTableName::FriendList),condition,update);

    // 5. 将message中的信息去除，防止占用空间
    condition = toString(DatabaseMessages::SenderId) + " = '" + friend_id + "' AND " + toString(DatabaseMessages::ReceiverId) + " = '" + client_id + "'";
    m_db_mysql->deleteRecord(toString(DatabaseTableName::Messages),condition);

    // 6. 构造发回的信息
    ChatLoadMessage msg(friend_id,info,array);

    // 6. 发送信息
    if(m_network_tcp->send(msg.getJsonObejct())){
        return true;
    }else{
        errorOccurred("cannot ChatMgr handleChatLoad");
        return false;
    }
}

/*
 * 2. 处理聊天信息
 */
bool ChatMgr::handleChatForward(const QJsonObject &json){

    // NOTICE 这里需要处理用户在线的情况，但是不确定是否大开，需要查看
    const QJsonObject& dialog = json[toString(JsonKey::ChatForward)].toObject();
    // 1. 判断是否打开
    const QString& receiver_id = dialog[toString(JsonChatForward::ReceiverId)].toString();
    const QString& sender_id = dialog[toString(JsonChatForward::SenderId)].toString();
    const QJsonObject& dia = dialog[toString(JsonChatForward::Dialog)].toObject();
    const QString& dia_time = dia[toString(JsonDialog::DialogTime)].toString();
    const QString& dia_content = dia[toString(JsonDialog::DiaLogContent)].toString();
    const QString& dia_sender_id = dia[toString(JsonDialog::DialogSenderId)].toString();

    QString condition = condition = toString(DatabaseFriendList::ClientId) + " = '" + receiver_id + "' AND " + toString(DatabaseFriendList::FriendId) + " = '" + sender_id + "'";
    const QMap<QString,QVariant>& receiver_info = m_db_mysql->selectRecord(toString(DatabaseTableName::FriendList),condition);
    if(receiver_info[toString(DatabaseFriendList::IsOpen)].toBool()){
        // 2. 如果打开，发送信息
        // NOTICE 无需存入Messages表，但是要修改lastMsgTime
        if(!(m_network_tcp->send(json))){
            errorOccurred("cannot ChatMgr handleChatForward");
            return false;   // NOTICE 一般不会发生这种情况，要事物处理完
        }

        // 3. 修改lastMsgTime表
        QMap<QString,QVariant> update;
        update.insert(toString(DatabaseFriendList::LastMsgTime),dia_time);
        m_db_mysql->updateRecord(toString(DatabaseTableName::FriendList),condition,update);
        condition = toString(DatabaseFriendList::ClientId) + " = '" + sender_id + "' AND " + toString(DatabaseFriendList::FriendId) + " = '" + receiver_id + "'";
        m_db_mysql->updateRecord(toString(DatabaseTableName::FriendList),condition,update);


    }else{
        // 4. 没有打开，更新信息
        ChatUpdateMessage msg(sender_id,dia_time);
        if(!(m_network_tcp->send(msg.getJsonObejct()))){
            errorOccurred("cannot ChatMgr handleChatForward2");
            return false;
        }

        // 5. 修改lastMsgTime、UnreadMsg
        condition = toString(DatabaseFriendList::ClientId) + " = '" + receiver_id + "' AND " + toString(DatabaseFriendList::FriendId) + " = '" + sender_id + "'";
        const QMap<QString,QVariant>& record = m_db_mysql->selectRecord(toString(DatabaseTableName::FriendList),condition);
        int unread_msg = record[toString(DatabaseFriendList::UnreadMsg)].toInt() + 1;   // 添加一条未读信息
        // 5. 1. 修改friend_id中对client_id信息：lastMsgTime和UnreadMsg
        QMap<QString,QVariant> update;
        update.insert(toString(DatabaseFriendList::LastMsgTime),dia_time);
        update.insert(toString(DatabaseFriendList::UnreadMsg),unread_msg);
        m_db_mysql->updateRecord(toString(DatabaseTableName::FriendList),condition,update);
        // 5. 2. 修改client_id中对friend_id的信息：lastMsgTime
        condition = toString(DatabaseFriendList::ClientId) + " = '" + sender_id + "' and " + toString(DatabaseFriendList::FriendId) + " = '" + receiver_id + "'";
        update.remove(toString(DatabaseFriendList::UnreadMsg));
        m_db_mysql->updateRecord(toString(DatabaseTableName::FriendList),condition,update);

        // 6. 存储Message信息
        condition = toString(DatabaseMessages::SenderId) + " = '" + sender_id + "' and " + toString(DatabaseMessages::ReceiverId) + " = '" + receiver_id + "' and " + toString(DatabaseMessages::ReceiverId) + " = '" + receiver_id + "'";
        QMap<QString,QVariant> msg_save;
        msg_save.insert(toString(DatabaseMessages::SenderId),dia_sender_id);
        msg_save.insert(toString(DatabaseMessages::ReceiverId),receiver_id);
        msg_save.insert(toString(DatabaseMessages::Time),dia_time);
        msg_save.insert(toString(DatabaseMessages::Content),dia_content);
        m_db_mysql->insertRecord(toString(DatabaseTableName::Messages),msg_save);
    }
    return true;
}

/*
 * 3. 处理聊天更新
 */
bool ChatMgr::handleChatClose(const QJsonObject &json){

    // 1. 获取id
    const QString& client_id = json[toString(JsonChatClose::ClientId)].toString();
    const QString& friend_id = json[toString(JsonChatClose::FriendId)].toString();

    // 2. 对数据库进行修改
    QString condition = toString(DatabaseFriendList::ClientId) + " = '" + client_id + "' and " + toString(DatabaseFriendList::FriendId) + " = '" + friend_id + "'";
    QMap<QString,QVariant> update;
    update.insert(toString(DatabaseFriendList::IsOpen),0);
    return m_db_mysql->updateRecord(toString(DatabaseTableName::FriendList),condition,update);  // 一般情况下都是正确的

}


void ChatMgr::errorOccurred(const QString &error){
    qDebug() << error;
}
