#include "FriendMgr.h"

/* * * * * * 1. FriendMgr对象的初始化 * * * * * */

/*
 * 1. 构造函数
 */
FriendMgr::FriendMgr(IDatabaseMySQL *db_mysql, INetworkTcp *network_tcp, QObject *parent)
    : IFriendMgr(parent), m_db_mysql(db_mysql), m_network_tcp(network_tcp){}

/*
 * 2. 析构函数
 */
FriendMgr::~FriendMgr(){
    qDebug() << "~FriendMgr called";
}





/* * * * * * 2. public 接口函数 * * * * * */

/*
 * 1. 消息加载函数
 */
bool FriendMgr::handleFriendListLoad(const QJsonObject &json){

    // 1. 构造条件
    const QString& client_id = json[toString(JsonFriendListLoad::ClientId)].toString();
    QString condition = toString(DatabaseFriendList::ClientId) + " = '" + client_id + "'";

    // 2. 获取friend_list中的信息
    const QList<QMap<QString,QVariant>>& friend_list = m_db_mysql->selectRecords(toString(DatabaseTableName::FriendList),condition);

    // 3. 读取每一个好友并存储起来
    QJsonArray json_array;
    for(const QMap<QString,QVariant>& friend_record: friend_list){

        const QString& friend_id = friend_record[toString(DatabaseFriendList::FriendId)].toString();
        condition = toString(DatabaseUserInfo::ClientId) + " = '" + friend_id + "'";
        const QMap<QString,QVariant>& friend_info = m_db_mysql->selectRecord(toString(DatabaseTableName::UserInfo),condition);

        // 如果后续有需要再进行封装
        QJsonObject json_friend;
        json_friend[toString(JsonFriend::FriendId)] = friend_id;
        json_friend[toString(JsonFriend::FriendNickname)] = friend_info[toString(DatabaseUserInfo::ClientNickname)].toString();
        json_friend[toString(JsonFriend::LastMsgTime)] = friend_record[toString(DatabaseFriendList::LastMsgTime)].toString();
        json_friend[toString(JsonFriend::UnreadMsg)] = friend_record[toString(DatabaseFriendList::UnreadMsg)].toInt();
        json_friend[toString(JsonFriend::IsOnline)] = friend_info[toString(DatabaseUserInfo::IsOnline)].toBool();  // 判断好友是否在线

        json_array.append(json_friend);
    }
    FriendListLoadMessage msg(json_array);
    if(m_network_tcp->send(msg.getJsonObejct())){
        return true;
    }else{
        errorOccurred("cannot handleFriendListLoad, cannot send()");
        return false;
    }
}

/*
 * 2. 好友添加函数
 */
bool FriendMgr::handleFriendListAdd(const QJsonObject &json){

    // 1. 查看是否存在该用户
    const QString& client1_id = json[toString(JsonFriendListAdd::ClientId)].toString();
    const QString& client2_id = json[toString(JsonFriendListAdd::FriendId)].toString();
    const QString& last_msg_time = json[toString(JsonFriendListAdd::LastMsgTime)].toString();
    QString condition = toString(DatabaseUserInfo::ClientId) + " = '" + client2_id + "'";
    const QMap<QString,QVariant>& fri_data = m_db_mysql->selectRecord(toString(DatabaseTableName::UserInfo),condition);

    if(!fri_data.isEmpty() && fri_data[toString(DatabaseUserInfo::ClientId)].toString() == client2_id){
        // 2. 用户存在，无需检查是否已经为好友，用户发出就说明不是

        // 3. 插入用户对盆友的好友关系
        QMap<QString,QVariant> relation;
        relation.insert(toString(DatabaseFriendList::ClientId),client1_id);
        relation.insert(toString(DatabaseFriendList::FriendId),client2_id);
        relation.insert(toString(DatabaseFriendList::LastMsgTime),json[toString(JsonFriendListAdd::LastMsgTime)].toString());
        m_db_mysql->insertRecord(toString(DatabaseTableName::FriendList),relation);

        // 4. 插入盆友对用户的好友关系
        relation[toString(DatabaseFriendList::ClientId)] = client2_id;
        relation[toString(DatabaseFriendList::FriendId)] = client1_id;
        m_db_mysql->insertRecord(toString(DatabaseTableName::FriendList),relation);

        // 5. 构造原客户端消息
        QJsonObject fri;
        fri[toString(JsonFriend::FriendId)] = client2_id;
        fri[toString(JsonFriend::FriendNickname)] = fri_data[toString(DatabaseUserInfo::ClientNickname)].toString();
        fri[toString(JsonFriend::LastMsgTime)] = last_msg_time;
        fri[toString(JsonFriend::UnreadMsg)] = 0;   // 默认发送信息，后续消息模块解决后要添加
        fri[toString(JsonFriend::IsOnline)] = fri_data[toString(DatabaseUserInfo::IsOnline)].toBool();  // 获取好友是否在线

        // 5. 1 此FriendMgr是在session中的，可以运行，说明没有被delete掉，所以不用担心是否在线，deletelater
        FriendListAddMessage msg(true,true,fri);
        if(!(m_network_tcp->send(msg.getJsonObejct()))){
            errorOccurred("cannot FriendMgr handleFriendListAdd, send error1");
            return false;
            //NOTICE 发送失败重新执行一遍而已，会有点开销，因为是在客户端检验的;
        }


        // 6. 构造好友客户端消息
        QJsonObject client;
        client[toString(JsonFriend::FriendId)] = client1_id;
        client[toString(JsonFriend::FriendNickname)] = json[toString(JsonFriendListAdd::ClientNickname)].toString();
        client[toString(JsonFriend::LastMsgTime)] = last_msg_time;
        client[toString(JsonFriend::UnreadMsg)] = 0;    // 同样需要发送信息
        client[toString(JsonFriend::IsOnline)] = true;  // 本客户端肯定在线

        // 6. 1 如果在线，发送信息
        FriendListAddMessage msg2(true,true,client);
        emit newFriendMgr(msg2.getJsonObejct(),client2_id);    // 通知好友的singlesession发送信息

        return true;

    }else{
        // 7. 用户不存在，返回错误信息
        FriendListAddMessage msg(false,false,QJsonObject{});  // 传入新空即可

        // 7. 1 原用户在该函数执行时肯定存在
        if(!(m_network_tcp->send(msg.getJsonObejct()))){
            errorOccurred("cannot FriendMgr handleFriendListAdd, send error3");
            return false;
        }
        // 如果不在线无需处理
        return true;
    }
}

/*
 * 3. 好友删除函数
 */
bool FriendMgr::handleFriendListDelete(const QJsonObject &json){

    // 1. 用户肯定存在，直接在好友列表中移除关系，两次
    const QString& client1_id = json[toString(JsonFriendListDelete::ClientId)].toString();
    const QString& client2_id = json[toString(JsonFriendListDelete::FriendId)].toString();

    QString condition = toString(DatabaseFriendList::ClientId) + " = '" + client1_id \
                        + "' AND " + toString(DatabaseFriendList::FriendId) + " = '" + client2_id + "'";
    if(m_db_mysql->deleteRecord(toString(DatabaseTableName::FriendList),condition)){
        condition = toString(DatabaseFriendList::ClientId) + " = '" + client2_id \
                    + "' AND " + toString(DatabaseFriendList::FriendId) + " = '" + client1_id + "'";
        if(m_db_mysql->deleteRecord(toString(DatabaseTableName::FriendList),condition)){

            // 删除client1_id给client2_id的消息
            condition = toString(DatabaseMessages::SenderId) + " = '" + client1_id \
                        + "' AND " + toString(DatabaseMessages::ReceiverId) + " = '" + client2_id + "'";
            m_db_mysql->deleteRecord(toString(DatabaseTableName::Messages),condition);
            // 删除client2_id给client1_id的消息
            condition = toString(DatabaseMessages::SenderId) + " = '" + client2_id \
                        + "' AND " + toString(DatabaseMessages::ReceiverId) + " = '" + client1_id + "'";
            m_db_mysql->deleteRecord(toString(DatabaseTableName::Messages),condition);


            // 2. 成功删除关系,构造客户端信息
            FriendListDeleteMessage client_msg(true,client2_id);

            // 当前用户肯定在线，这种情况应该不会发生
            if(!(m_network_tcp->send(client_msg.getJsonObejct()))){
                errorOccurred("cannot FriendMgr handleFriendListDelete, cannot send");
                return false;
            }

            // 3. 构造好友关系，如果好友在线发送
            FriendListDeleteMessage fri_msg(true,client1_id);
            emit delFriendMgr(fri_msg.getJsonObejct(),client2_id);


        }else{
            errorOccurred("cannot FriendMgr handleFriendListDetele, cannot deleteRecord2");
        }

    }else{
        errorOccurred("cannot FriendMgr handleFriendListDelete, cannot deleteRecord1");
    }
    return true;
}

/*
 *  好友列表登录更新
 */
bool FriendMgr::handleFriendListUpdate(const QJsonObject &json){
    return (m_network_tcp->send(json));
}

/*
 * 好友信息更新
 */
bool FriendMgr::handleFriendListSetting(const QJsonObject &json){
    return (m_network_tcp->send(json));
}

/*
 * 4. 别人添加好友时向自己发送信息
 */
bool FriendMgr::handleNewFriendComing(const QJsonObject &json){
    // 1. 此时用户可以进行业务，说明network有连接，无需判断
    return (m_network_tcp->send(json));
}

/*
 * 5. 别人删除好友时向自己发送信息
 */
bool FriendMgr::handleDelFriendComing(const QJsonObject &json){
    // 1. 说明有链接
    return (m_network_tcp->send(json));
}

/*
 * 错误处理
 */
void FriendMgr::errorOccurred(const QString& error){
    qDebug() << error;
}
