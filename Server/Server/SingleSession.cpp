#include "SingleSession.h"

/* * * * * * 1. SingleSession的初始化和销毁 * * * * * */

/*
 * 1. 构造函数
 */
SingleSession::SingleSession(const QString& client_id, INetworkTcp* network_tcp, IDatabaseMySQL* db_mysql, QObject* parent)
    : QObject{parent},m_client_id(client_id), m_user_authen{new UserAuthen(db_mysql,network_tcp,this)},
    m_friend_mgr{new FriendMgr(db_mysql,network_tcp,this)}, m_chat_mgr(new ChatMgr(network_tcp,db_mysql,this)){


    // 1. 重新设置network_tcp父对象
    network_tcp->setParent(this);
    // 使用外部传入的IDatabaseMySQL和外部的INetworkTcp来新创建一个UserAuthen，父对象析构时这个是子对象中最晚析构的，符合

    // 2. 进行连接
    connect(m_friend_mgr,&IFriendMgr::newFriendMgr,this,&SingleSession::do_newFriendMgr);
    connect(m_friend_mgr,&IFriendMgr::delFriendMgr,this,&SingleSession::do_delFriendMgr);
}


/*
 * 2. 析构函数
 */
SingleSession::~SingleSession(){
    // 退出的时候会去除，已经登录后无法有多个相同id的session，不会被其他修改，未登录时多次修改没问题
    // 父对象机制是先父对象再子对象，可以使用模块来解决
    m_user_authen->logoutUser(m_client_id); // 自己退出
    qDebug() << "~SingleSession called";
}


/* * * * * * 2. public接口函数实现 * * * * * */

/*
 * 1. getId
 */
QString SingleSession::getId(){
    return m_client_id;
}

/*
 * 2. 注册
 */
bool SingleSession::registerSession(const QJsonObject &json){

    // 1. 进行json内容解析
    const QString& client_id = json[toString(JsonRegister::ClientId)].toString();
    const QString& client_password = json[toString(JsonRegister::ClientPassword)].toString();

    if(m_user_authen->RegisterUser(client_id,client_password)){
        return true;
    }else{
        errorOccurred("cannot SingleSession registerSession");
        return false;
    }
}

/*
 * 3. 登录
 */
bool SingleSession::loginSession(const QJsonObject &json){

    // 1. 对json内容进行解析
    const QString& client_id = json[toString(JsonLogin::ClientId)].toString();
    const QString& client_password = json[toString(JsonLogin::ClientPassword)].toString();

    if(m_user_authen->loginUser(client_id,client_password)){
        return true;
    }else{
        errorOccurred("cannot SingleSession loginSession");
        return false;
    }
}

/*
 */
bool SingleSession::logoutSession(){
    return m_user_authen->logoutUser(m_client_id);

}

/*
 * 4. 处理好友加载
 */
bool SingleSession::FriendListLoad(const QJsonObject &json){
    return (m_friend_mgr -> handleFriendListLoad(json));
}

/*
 * 5. 处理好友添加
 */
bool SingleSession::FriendListAdd(const QJsonObject &json){
    return (m_friend_mgr->handleFriendListAdd(json));
}

/*
 * 6. 处理好友删除
 */
bool SingleSession::FriendListDelete(const QJsonObject &json){
    return (m_friend_mgr->handleFriendListDelete(json));
}

/*
 * 7. 处理好友列表更新
 * 功能：好友登录
 */
bool SingleSession::FriendListUpdate(const QJsonObject &json){
    return (m_friend_mgr->handleFriendListUpdate(json));
}


/*
 * 8. 好友信息重新设置，比如备注等
 */
bool SingleSession::FriendListSetting(const QJsonObject &json){
    return (m_friend_mgr->handleFriendListSetting(json));
}

/*
 * 7. 转发别人的好友信息
 */
bool SingleSession::newFriendComing(const QJsonObject &json){
    return (m_friend_mgr->handleNewFriendComing(json));
}

/*
 * 8. 转发别人的删除好友
 */
bool SingleSession::delFriendComing(const QJsonObject &json){
    return (m_friend_mgr->handleDelFriendComing(json));
}

/*
 * 9. 处理chat加载信息
 */
bool SingleSession::chatLoad(const QJsonObject &json){
    return (m_chat_mgr->handleChatLoad(json));
}


/*
 * 10. 处理chat发送信息
 */
bool SingleSession::chatForward(const QJsonObject &json){
    return (m_chat_mgr->handleChatForward(json));
}


/*
 * 11. 处理chat关闭信息
 */
bool SingleSession::chatClose(const QJsonObject &json){
    return (m_chat_mgr->handleChatClose(json));
}

/* * * * * * 3. private slots槽函数 * * * * * */

/*
 * 1. do_newFriendMgr()
 * 功能：向朋友的用户端发送添加消息
 */
void SingleSession::do_newFriendMgr(const QJsonObject &json, const QString &friend_id){
    emit newFriendSession(json,friend_id);
}

/*
 * 2. do_delFriendMgr()
 * 功能：向盆友的用户端发送删除消息
 */
void SingleSession::do_delFriendMgr(const QJsonObject &json, const QString &friend_id){
    emit delFriendSession(json,friend_id);
}

/*
 * 错误处理函数
 */
void SingleSession::errorOccurred(const QString &error){
    qDebug() << error;
}
