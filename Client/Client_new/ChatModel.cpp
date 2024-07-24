#include "ChatModel.h"

/* * * * * * 1. 对象初始化和销毁 * * * * * */

/*
 * 1. 构造函数
 * 改进：
 *      1. 这里默认加载历史记录的函数总是执行成功
 */
ChatModel::ChatModel(const QString& friend_id, QObject* parent)
    : IChatModel(parent), m_new_dialog(0), m_friend_id(friend_id)
{
    loadLocalDialog();
}

/*
 * 2. 析构函数
 */
ChatModel::~ChatModel(){
    saveLocalDialog();
    qDebug() << "~ChatModel called";
}

/* * * * * * 2. 接口函数实现 * * * * * */

/*
 * 1. rowCount函数
 */
int ChatModel::rowCount(const QModelIndex &parent) const{

    if(parent.isValid()){
        return 0;
    }
    return m_dialogs.size();
}


/*
 * 2. data函数
 */
QVariant ChatModel::data(const QModelIndex &index, int role) const{

    // 1. 检查数据有效性

    if(!index.isValid()){
        return QVariant();
    }

    if(index.row() >= m_dialogs.size() || index.row() < 0){
        return QVariant();
    }

    // 获取数据
    const Dialog& msg = m_dialogs[index.row()];
    // 根据角色返回数据
    switch(role){
    case TimeRole:
        return msg.dialog_time;
    case SenderIdRole:
        return msg.dialog_sender_id;
    case ContentRole:
        return msg.dialog_content;
    default:
        return QVariant();
    }
}

/*
 * 3. roleNames函数
 */
QHash<int, QByteArray> ChatModel::roleNames() const{

    QHash<int, QByteArray> roles;
    roles[TimeRole] = "dialog_time";
    roles[SenderIdRole] = "dialog_sender_id";
    roles[ContentRole] = "dialog_content";
    return roles;
}


/*
 * 4. 外部接口函数1 处理收到的加载信息
 * 功能：处理在friendList中接受到的好友聊天信息
 */
bool ChatModel::handleChatLoad(const QJsonObject &json){

    QJsonArray json_array = json[toString(JsonChatLoad::Dialogs)].toArray();

    for(const QJsonValue& item : json_array){
        const QJsonObject& dialog = item.toObject();
        const QString& dialog_time = dialog[toString(JsonDialog::DialogTime)].toString();
        const QString& dialog_sender_id = m_friend_id;
        const QString& dialog_content = dialog[toString(JsonDialog::DiaLogContent)].toString();
        m_dialogs.append(Dialog(dialog_time,dialog_sender_id,dialog_content));
    }

    return true;
}

/*
 * 5. 处理消息的添加
 */
bool ChatModel::addDialog(const Dialog &dialog){

    beginInsertRows(QModelIndex(),m_dialogs.size(),m_dialogs.size());
    m_dialogs.append(dialog);
    endInsertRows();
    return true;
}



/* * * * * * 3. private辅助函数实现 * * * * * */

/*
 * 1. 加载信息
 * 改进:
 *      1. 没有处理文件创建失败和读取失败和无法创建目录的情况——false
 *      2. 这里使用QFile和QDir，后续有时间在补上标准cpp的版本
 */
bool ChatModel::loadLocalDialog(){

    QString data_dir = "dialogs";
    QString current_dir = QDir::currentPath();
    // qDebug() << current_dir;
    QDir dir;
    if(!dir.exists(data_dir)){
        if(!dir.mkdir(data_dir)){

            // 如果目录不存在则创建目录
            qDebug() << "无法创建目录";

            return false;   // 无法创建，后续处理
        }
    }

    // arg是QString的成员方法，必须QString，qt中字面量默认是cstring
    QString data_path = QString("./dialogs/%1to%2.bin").arg(User::getInstance()->getId()).arg(m_friend_id);    // 指定id文件
    QFile file(data_path);
    // 查看文件是否存在
    if(!file.exists()){
        // 文件不存在，创建文件
        if(!file.open(QIODevice::WriteOnly)){

            qDebug() << "cannot ChatModel loadLocaldialog";
            return false; // 后续需要检查，文件无法创建
        }else{
            file.close();   // 直接关闭，这里只需要创建
            return true; // 无需读取信息，因为根本没有信息
        }
    }else{
        // 如果文件存在，读取二进制信息并转化为dialog对象存到m_dialogs中
        if(!file.open(QIODevice::ReadOnly)){
            return false; // 后续需要检查，文件无法读取
        }else{

            // 从二进制文件中读取信息
            QDataStream fin(&file);
            fin.setVersion(QDataStream::Qt_6_3);

            while(!fin.atEnd()){
                Dialog dialog;
                // 使用QDataStream读取二进制文件就如同读取文本文件一样，有加持
                fin >> dialog.dialog_time >> dialog.dialog_sender_id >> dialog.dialog_content;
                // if(msg.dialog_msg == "end" && msg.dialog_sender_id == "end" && msg.dialog_time == "end"){
                //     break;  // 读取到标志内容，退出
                // }
                m_dialogs.append(dialog); // 不断存入
                m_new_dialog += 1; // 加载了旧信息
            }
            file.close();
            return true;
        }
    }
}



/*
 * 2. 保存信息
 * 改进：
 *      1. 中间那么多步骤就是为了防止中途删除的情况出现
 */
bool ChatModel::saveLocalDialog(){


    // 1. 目录检查
    QString data_dir = "dialogs";
    QDir dir;   // 默认当前目录
    if(!dir.exists(data_dir)){
        if(!dir.mkdir(data_dir)){
            // 如果目录不存在则创建目录
            qDebug() << "cannot ChatModel saveLocaldialog, cannot mkdir";

            return false;   // 无法创建，后续处理
        }
    }

    // 2 目录检查
    QString data_path = QString("./dialogs/%1to%2.bin").arg(User::getInstance()->getId()).arg(m_friend_id); // arg是QString的成员方法，必须QString，qt中字面量默认是cstring
    QFile file(data_path);
    // 查看文件是否存在
    if(!file.exists()){
        // 文件不存在，创建文件
        if(!file.open(QIODevice::WriteOnly)){
            return false; // 后续需要检查，文件无法创建
        }

        // 说明成功创建，进行存入

    }else{
        // 如果文件存在，读取二进制信息并转化为dialog对象存到m_dialogs中
        if(!file.open(QIODevice::Append)){
            return false; // 后续需要检查，文件存在但是无法打开
        }
    }

    // 3. 追加信息
    // 到这一步是刚创建或者补充的
    QDataStream fout(&file);
    fout.setVersion(QDataStream::Qt_6_3);

    for(int i = m_new_dialog; i < m_dialogs.size(); i++){
        const Dialog& dialog = m_dialogs[i];
        fout << dialog.dialog_time << dialog.dialog_sender_id << dialog.dialog_content;
    }

    // // 写入结束标志
    // fout << "end" << "end" << "end";

    file.close();
    return true;
}
