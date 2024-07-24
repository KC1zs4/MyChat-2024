#include "FriendListModel.h"

/* * * * * * * 1. FriendListModel对象初始化 * * * * * */

/*
 * 1. 构造函数
 */
FriendListModel::FriendListModel(QObject *parent)
    : IFriendListModel(parent){}

/*
 * 2. 析构函数
 */
FriendListModel::~FriendListModel(){
    qDebug() << "~FriendListModel called";
}




/* * * * * * 2. QAbstractListModel接口函数 * * * * * */

/*
 * 1. rowCount
 */
int FriendListModel::rowCount(const QModelIndex &parent) const{
    if(parent.isValid()){
        return 0;
    }else{
        return m_friends.size();
    }
}

/*
 * 2. 返回数据
 */
QVariant FriendListModel::data(const QModelIndex &index, int role) const{

    if(!index.isValid() || index.row() >= m_friends.size() || index.row() < 0){
        return QVariant();
    }

    const Friend& fri = m_friends.at(index.row());
    switch(role){
    case IdRole:
        return fri.friend_id;
    case NicknameRole:
        return fri.friend_nickname;
    case IsOnlineRole:
        return fri.friend_is_online;
    case LastMsgTimeRole:
        return fri.friend_last_msg_time;
    case UnReadMsgRole:
        return fri.friend_unread_msg;
    default:
        return QVariant();
    }

}

/*
 * 3. 返回角色名
 */
QHash<int,QByteArray> FriendListModel::roleNames() const {
    QHash<int,QByteArray> roles = {
        {IdRole,"friend_id"},
        {NicknameRole,"friend_nickname"},
        {IsOnlineRole,"friend_is_online"},
        {LastMsgTimeRole,"friend_last_msg_time"},
        {UnReadMsgRole,"friend_unread_msg"}
    };
    return roles;
}



/* * * * * * 3. public 接口函数 * * * * * */


/*
 * 1. 处理好友列表加载
 */
bool FriendListModel::handleFriendListLoad(const QJsonObject &json){

    QJsonArray friends_array = json[toString(JsonFriendListLoad::Friends)].toArray();
    beginResetModel();
    for(const QJsonValue& value: friends_array){

        const QJsonObject& friend_json = value.toObject();
        const Friend& fri = Friend::fromJson(friend_json);
        m_friends.append(fri);
    }
    endResetModel();

    return true;
}

/*
 * 2. 处理好友列表添加
 */
bool FriendListModel::handleFriendListAdd(const QJsonObject &json){

    // 1. 判断添加是否成功
    if(json[toString(JsonFriendListAdd::Success)].toBool()){

        const QJsonObject& fri_json = json[toString(JsonFriendListAdd::Friend)].toObject();
        const Friend& fri = Friend::fromJson(fri_json);
        beginInsertRows(QModelIndex(),m_friends.size(),m_friends.size());
        m_friends.push_back(fri);
        endInsertRows();
        return true;

    }else{
        // 加入失败
        if(!json[toString(JsonFriendListAdd::Exist)].toBool()){
            return false;
        }
        return false;
    }
}

/*
 * 3. 处理好友列表删除
 */
bool FriendListModel::handleFriendListDelete(const QJsonObject &json){

    // 1. 判断是否删除成功
    if(json[toString(JsonFriendListDelete::Success)].toBool()){
        // 2. 成功删除
        const QString& friend_id = json[toString(JsonFriendListDelete::FriendId)].toString();
        int index = -1;
        int counter = 0;
        for(const Friend& fri:m_friends){
            if(fri.friend_id == friend_id){
                index = counter;
                break;
            }
            counter++;
        }
        if(index!=-1){  // 找到了index
            beginRemoveRows(QModelIndex(),index,index);
            m_friends.remove(index);
            endRemoveRows();

            // // 从文件系统中删除记录
            // deleteLocalDialog(friend_id);

            return true;
        }else{
            return false;
        }

    }else{
        return false;
    }
}

/*
 * 处理好友登录更新列表
 */
bool FriendListModel::handleFriendListUpdate(const QJsonObject &json){
    const QString& friend_id = json[toString(JsonFriendListUpdate::FriendId)].toString();
    bool is_online = json[toString(JsonFriendListUpdate::IsOnline)].toBool();

    for(auto iter = m_friends.begin();iter!=m_friends.end();iter++){
        if(iter->friend_id == friend_id){
            iter->friend_is_online = is_online;
            int index = std::distance(m_friends.begin(),iter);
            emit dataChanged(this->index(index),this->index(index));
        }
    }
    return true;
}

/*
 * 4. 处理好友信息更新
 */
bool FriendListModel::handleFriendListSetting(const QJsonObject &json){
    const QString& friend_id = json[toString(JsonFriendListSetting::ClientId)].toString();
    const QString& new_nickname = json[toString(JsonFriendListSetting::ClientNickname)].toString();
    for(auto iter = m_friends.begin();iter!=m_friends.end();iter++){
        if(iter->friend_id == friend_id){
            iter->friend_nickname = new_nickname;
            int index = std::distance(m_friends.begin(),iter);
            emit dataChanged(this->index(index),this->index(index));
        }
    }
    return true;
}

/*
 * 4. 好友信息更新
 */
bool FriendListModel::handleChatUpdate(const QJsonObject &json){
    const QString& friend_id = json[toString(JsonChatUpdate::SenderId)].toString();
    const QString& last_msg_time = json[toString(JsonChatUpdate::LastMsgTime)].toString();

    for(auto iter = m_friends.begin();iter!=m_friends.end();iter++){
        if(iter->friend_id == friend_id){
            iter->friend_last_msg_time = last_msg_time;
            iter->friend_unread_msg++;
            int index = std::distance(m_friends.begin(),iter);
            emit dataChanged(this->index(index),this->index(index));
        }
    }
    return true;    // 没有考虑到不改变的情形
}

/*
 * 5. 发送好友信息
 */
void FriendListModel::handleChatWindowSendDialog(const QString &friend_id, const QString &time){
    for(auto iter = m_friends.begin();iter!=m_friends.end();iter++){
        if(iter->friend_id == friend_id){
            iter->friend_last_msg_time = time;
            // iter->friend_unread_msg = 0; 无需修改
            int index = std::distance(m_friends.begin(),iter);
            emit dataChanged(this->index(index),this->index(index));
        }
    }
}

/*
 * 6. 打开好友列表消除红点
 */
void FriendListModel::handleChatWindowTime(const QString &friend_id){
    for(auto iter = m_friends.begin();iter!=m_friends.end();iter++){
        if(iter->friend_id == friend_id){
            iter->friend_unread_msg = 0;
            int index = std::distance(m_friends.begin(),iter);
            emit dataChanged(this->index(index),this->index(index));
        }
    }
}

/*
 * 5. 是否存在
 */
bool FriendListModel::isContainFriend(const QString &friend_id) const {
    for(const Friend& fri : m_friends){
        if(fri.friend_id==friend_id){
            return true;
        }
    }
    return false;
}


// /* * * * * * 3. private方法 * * * * * */

// // 由于没有时间通知好友进行修改，所以暂时注释，后续迭代添加
// bool FriendListModel::deleteLocalDialog(const QString &friend_id){
//     QString data_dir = "dialogs";
//     QDir dir;
//     if(!dir.exists(data_dir)){
//         return true;    // 不存在历史记录目录，没有关系
//     }else{
//         // 1. 查看是否存在文件
//         QString data_path = QString("./dialogs/%1to%2.bin").arg(User::getInstance() -> getId()).arg(friend_id);
//         QFile file(data_path);

//         if(!file.exists()){
//             return true;
//         }else{
//             // 2. 删除文件
//             if(file.remove()){
//                 return true;
//             }else{
//                 errorOccurred("cannot FriendListModel deleteLocalDialog");
//                 return false;
//             }
//         }
//     }
// }

/*
 * 错误处理
 */
void FriendListModel::errorOccurred(const QString &error) const{
    qDebug() << error;
}
