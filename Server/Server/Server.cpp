#include "Server.h"

/* * * * * * 1. Server的初始化和销毁 * * * * * */

/*
 * 1. 构造函数
 */
Server::Server(QObject *parent)
    : QObject{parent}, m_tcp_server{new QTcpServer(this)}, m_running_port(8080), m_set_port(8080),
    m_db_mysql{new DatabaseMySQL("127.0.0.1",3306,"root","?????","qq",this)}{

    // 1. 连接信号和槽
    connect(m_tcp_server,&QTcpServer::newConnection,this,&Server::do_newConnection);
}

/*
 * 2. 析构函数
 */
Server::~Server(){
    qDebug() << "~Server called";
}


/* * * * * * 2. 公共接口函数 * * * * * */

/*
 * 1. 开启服务器
 */
bool Server::startServer(){
    if(m_tcp_server->isListening()){
        std::cout << "----Success, the server is already running\n";
        return true;
    }else{
        if(m_tcp_server->listen(QHostAddress::LocalHost,m_set_port)){
            std::cout << "Success, the server is running\n";
            // std::cout << "----Success, the server is running\n";
            m_running_port = m_set_port;
            return true;
        }else{
            std::cout << "----Failed, the server is not running\n";
            errorOccurred("cannot Server startServer(), can't listen");
            return false;
        }
    }
}

/*
 * 2. 关闭服务器
 */
bool Server::stopServer(){
    if(m_tcp_server->isListening()){
        m_tcp_server->close();
        if(m_tcp_server->isListening()){
            std::cout << "----Failed, can't stop the server\n";
            errorOccurred("cannot Server stopServer()");
            return false;
        }else{
            std::cout << "----Success, the server is closed\n";
            return true;
        }
    }else{
        std::cout << "----Success, the server is not running\n";
        return true;
    }
}

/*
 * 3. 设置端口
 */
bool Server::setPort(){
    quint16 temp_port;
    std::cout << "----Please input your port: ";
    std::cin >> temp_port;
    std::cin.get();
    if(temp_port>0){
        m_set_port = temp_port;
        std::cout << "----Success, your port has been set: " << m_set_port << "\n";
        return true;
    }else{
        std::cout << "----Failed, illegal port number\n";
        errorOccurred("cannot Server setPort()");
        return false;
    }
}


/*
 * 4. 显示状态
 */
void Server::showStatus() const{
    std::cout
        << "----Status: \n"
        << "----1. Server " << (m_tcp_server->isListening() ? "is running on " + std::to_string(m_running_port) : "is not running") << "\n"
        << "----2. Set Port: " << m_set_port << "\n";
    return;
}



/* * * * * * 3. private slots槽函数实现 * * * * * */

/*
 * 1. do_newConnection
 */
void Server::do_newConnection(){
    QTcpSocket* tcp_socket = m_tcp_server -> nextPendingConnection();   // 获取要连接客户端的socket
    if(tcp_socket){

        // 此时只是建立新连接，还未发送信息

        // socket已经找到，建立槽函数
        INetworkTcp* tcp_network = new NetworkTcp(tcp_socket,this); // 先以Server为父对象，后面再改为session来表示
        connect(tcp_network,&INetworkTcp::received,this,&Server::do_received);  // NetworkTcp发送信号后触发Server的函数
        connect(tcp_network,&INetworkTcp::disconnectNetworkTcp,this,&Server::do_disconnectedNetworkTcp);    // NetworkTcp发送信号后触发Server函数

    }else{
        errorOccurred("cannot Server do_newConnection");
    }
}


/*
 * 2. do_disconnected
 */
void Server::do_disconnectedNetworkTcp(){


    // 1. 处理会话
    INetworkTcp* network_tcp = qobject_cast<INetworkTcp*>(sender());
    // 不一定存在对应的session

    SingleSession* single_session = m_single_sessions.value(network_tcp);
    if(single_session==nullptr){
        // 2. 没有对应的session就断开连接了,network有指定父对象，可以不用手动内存管理
    }
    // 3. 存在对应session
    m_single_sessions.remove(network_tcp);
    const QString& client_id = single_session->getId();    // 获取session对应的id
    if(m_single_sessions_online.contains(client_id)){
        m_single_sessions_online.remove(client_id);
        // 发送消息，退出了，变灰
        handleFriendListUpdate(client_id,false);
    }
    single_session->deleteLater();

}

/*
 * 3. do_received
 */
void Server::do_received(const QJsonObject& json){

    // 对json信息进行处理和分发，这里可以考虑使用msg进行封装

    const QString& type = json[toString(JsonKey::Type)].toString();
    const QString& action = json[toString(JsonKey::Action)].toString();
    if(type == toString(JsonType::UserAuthen)){
        // 1. 类型是用户验证，比较特殊，在server中有helperfunc，因为设计qmap

        INetworkTcp* network_tcp = qobject_cast<INetworkTcp*>(sender());
        if(action == toString(JsonAction::Register)){
            const QJsonObject& registr = json[toString(JsonKey::Register)].toObject();
            handleRegister(registr,network_tcp);
        }else if(action == toString(JsonAction::Login)){
            const QJsonObject& login = json[toString(JsonKey::Login)].toObject();
            handleLogin(login,network_tcp);
        }
    }else if(type == toString(JsonType::Friend)){

        // 2. 类型是好友，其实就是好友列表，获取id的session后进行处理
        // NOTICE 先获取id，要区分在线和不在线两种情况

        if(action == toString(JsonAction::Load)){
            const QJsonObject& load = json[toString(JsonKey::FriendListLoad)].toObject();
            const QString& client_id = load[toString(JsonFriendListLoad::ClientId)].toString();

            SingleSession* single_session = m_single_sessions_online.value(client_id);
            if(single_session == nullptr){
                return;
            }else{
                single_session -> FriendListLoad(load);
            }

        }else if(action == toString(JsonAction::Add)){
            const QJsonObject& add = json[toString(JsonKey::FriendListAdd)].toObject();
            const QString& client_id = add[toString(JsonFriendListAdd::ClientId)].toString();

            SingleSession* single_session = m_single_sessions_online.value(client_id);
            if(single_session == nullptr){
                return;
            }else{
                single_session -> FriendListAdd(add);
            }

        }else if(action == toString(JsonAction::Delete)){
            const QJsonObject& del = json[toString(JsonKey::FriendListDetete)].toObject();
            const QString& client_id = del[toString(JsonFriendListDelete::ClientId)].toString();

            SingleSession* single_session = m_single_sessions_online.value(client_id);
            if(single_session == nullptr){
                return;
            }else{
                single_session -> FriendListDelete(del);
            }
        }
    }else if(type == toString(JsonType::Setting)){

        INetworkTcp* network_tcp = qobject_cast<INetworkTcp*>(sender());

        if(json[toString(JsonKey::Action)] == toString(JsonAction::Save)){

            const QJsonObject& save = json[toString(JsonKey::SettingSave)].toObject();
            handleSettingSave(save,network_tcp);

        }else if(json[toString(JsonKey::Action)] == toString(JsonAction::Logout)){

            const QJsonObject& logout = json[toString(JsonKey::SettingLogout)].toObject();
            handleSettingLogout(logout,network_tcp);
        }
    }else if(type == toString(JsonType::Chat)){

        if(json[toString(JsonKey::Action)].toString() == toString(JsonAction::Load)){
            const QJsonObject& load = json[toString(JsonKey::ChatLoad)].toObject();
            const QString& client_id = load[toString(JsonChatLoad::ReceiverId)].toString();
            SingleSession* single_session = m_single_sessions_online.value(client_id);
            if(single_session == nullptr){
                return;
            }else{
                single_session -> chatLoad(load);
            }
        }else if(json[toString(JsonKey::Action)].toString() == toString(JsonAction::Forward)){
            const QJsonObject& forward = json[toString(JsonKey::ChatForward)].toObject();
            const QString& friend_id = forward[toString(JsonChatForward::ReceiverId)].toString();
            SingleSession* single_session = m_single_sessions_online.value(friend_id);
            if(single_session == nullptr){
                // 说明用户不在线，调用Server端进行处理
                handleChatForwardOff(forward);
            }else{
                single_session -> chatForward(json);    // 用户在线，有两种情况，打开和没有打开，内部处理
            }
        }else if(json[toString(JsonKey::Action)].toString() == toString(JsonAction::Close)){
            const QJsonObject& close = json[toString(JsonKey::ChatClose)].toObject();
            const QString& client_id = close[toString(JsonChatClose::ClientId)].toString();
            SingleSession* single_session = m_single_sessions_online.value(client_id);
            if(single_session!=nullptr){
                // 用户在线
                single_session->chatClose(close);

            }else{
                return; // NOTICE 一般不会有这种情况
            }
        }
    }
    // 后续再补充

    // 1. 有NetworkTcp对象发送而来

}


/*
 * 4. 处理好友添加消息
 * 功能：添加完互相关系后，需要将对方的好友列表也更新
 */
void Server::do_newFriendSession(const QJsonObject &json, const QString &friend_id){

    // 1. 往friend_id发送信息
    SingleSession* single_session = m_single_sessions_online.value(friend_id);
    if(single_session!=nullptr){
        // 2. 好友在线，发送信息
        single_session -> newFriendComing(json);
    }else{
        // 3. 消息进行缓存
    }
}

/*
 * 5. 处理好友删除的信息
 * 功能：删除关系后，使得对方好友列表也更新
 */
void Server::do_delFriendSession(const QJsonObject &json, const QString &friend_id){

    // 1. 往friend_id发送信息
    SingleSession* single_session = m_single_sessions_online.value(friend_id);
    if(single_session!=nullptr){
        // 2. 好友在在线，发送信息
        single_session->delFriendComing(json);
    }else{
        // 无需进行缓存，只需要通知改变而已
    }
}




/* * * * * * 3. 私有辅助函数 * * * * * */

/*
 * 1. 注册
 */
bool Server::handleRegister(const QJsonObject &json, INetworkTcp* network_tcp){

    const QString& client_id = json[toString(JsonRegister::ClientId)].toString();

    // 1. 发送消息来的NetworkTcp是否已经存在对应的session，不需要判断是否在线，注册只看是否已经存在账号了
    SingleSession* single_session = m_single_sessions.value(network_tcp);
    if(single_session!=nullptr){
        // 2. 存在对应的session，判断是否为同一个会话
        if(single_session->getId() == client_id){
            // 3. 是同一个会话，可以直接使用该session进行操作
            if(single_session->registerSession(json)){
                return true;
            }else{
                errorOccurred("cannot Server handleRegister, registerSession not success");
                return false;
            }
        }else{
            // 4. 不是同一个对话，需要重新建立session
            m_single_sessions.remove(network_tcp);
            single_session->deleteLater();
        }
    }

    // 5. 创建新的single_session
    single_session = new SingleSession(client_id,network_tcp,m_db_mysql,this);
    connect(single_session,&SingleSession::newFriendSession,this,&Server::do_newFriendSession);
    connect(single_session,&SingleSession::delFriendSession,this,&Server::do_delFriendSession);
    // NOTICE 此处single_session设置server为父对象，防止delete失败内存泄露
    // NOTICE 此处的所有的会话都使用同一个数据库连接对象
    // NOTICE 此处内部的user_authen已经将父对象设置为single_session了，同时修改了network_tcp的父对象，并不推荐修改父对象

    // 6. 进行single_session接口部分
    m_single_sessions.insert(network_tcp,single_session);
    if(single_session->registerSession(json)){
        return true;
    }else{
        errorOccurred("cannot Server handleRegister, registerSession failed");
        return false;
    }
}

/*
 * 2. 登录
 */
bool Server::handleLogin(const QJsonObject &json, INetworkTcp *network_tcp){

    const QString& client_id = json[toString(JsonLogin::ClientId)].toString();

    // 1. 查看用户是否已经登录
    SingleSession* single_session = m_single_sessions_online.value(client_id);
    if(single_session!=nullptr){
        // 2. 用户已经登录
        LoginMessage msg(false,true,true,false,QJsonObject{});    // 用户已经登录
        network_tcp->send(msg.getJsonObejct());
        return false;
    }else{

        // 3. 用户没有登录
        single_session = m_single_sessions.value(network_tcp);
        if(single_session!=nullptr){
            // 4. 之前存在有一个single_session，和上次是同一个
            if(single_session->getId() == client_id){
                // 5. 是同一个id，可以重用
                if(single_session->loginSession(json)){
                    m_single_sessions_online.insert(client_id,single_session);
                    // 通知其他好友同步同步进行显示为白色
                    handleFriendListUpdate(client_id,true);
                    return true;
                }else{
                    errorOccurred("cannot Server handleLogin, loginSession failed");
                    return false;
                }
            }else{
                // 6. 不是同一个id，不可以重用
                m_single_sessions.remove(network_tcp);
                single_session->deleteLater();
            }
        }

        // 7. 创建新的single_session
        single_session = new SingleSession(client_id,network_tcp,m_db_mysql,this);
        // 8. 进行信号的连接
        connect(single_session,&SingleSession::newFriendSession,this,&Server::do_newFriendSession);
        connect(single_session,&SingleSession::delFriendSession,this,&Server::do_delFriendSession);

        m_single_sessions.insert(network_tcp,single_session);
        if(single_session->loginSession(json)){
            m_single_sessions_online.insert(client_id,single_session);
            handleFriendListUpdate(client_id,true);
            return true;
        }else{
            errorOccurred("cannot Server handleLogin, loginSession failed2");
            return false;
        }
    }
}

/*
 * 3. 进行设置
 */
bool Server::handleSettingSave(const QJsonObject& json, INetworkTcp* network_tcp){

    const QString& client_id = json[toString(JsonSettingSave::ClientId)].toString();
    const QString& client_nickname = json[toString(JsonSettingSave::ClientNickname)].toString();
    const QString& client_signature = json[toString(JsonSettingSave::ClientSignature)].toString();
    const QString& client_email = json[toString(JsonSettingSave::ClientEmail)].toString();
    QString condition = toString(DatabaseUserInfo::ClientId) + " = '" + client_id + "'";
    QMap<QString,QVariant> info;
    info.insert(toString(DatabaseUserInfo::ClientNickname),client_nickname);
    info.insert(toString(DatabaseUserInfo::ClientEmail),client_email);
    info.insert(toString(DatabaseUserInfo::ClientSignature),client_signature);

    bool success = m_db_mysql->updateRecord(toString(DatabaseTableName::UserInfo),condition,info);
    SettingSaveMessage msg(success,(success?json:QJsonObject{}));
    if(network_tcp->send(msg.getJsonObejct())){
        // 往好友发送自己改变的事情
        handleFriendListSetting(client_id,client_nickname,client_email,client_signature);
        return true;
    }else{
        errorOccurred("cannot Server handleSettingSave");
        return false;
    }
}

/*
 * 4. 进行登出
 */
bool Server::handleSettingLogout(const QJsonObject &json, INetworkTcp* network_tcp){

    // 1. 进行退出操作
    const QString& client_id = json[toString(JsonSettingLogout::ClientId)].toString();
    SingleSession* single_session = m_single_sessions_online.value(client_id);
    if(single_session==nullptr){
        // NOTICE 一般不会出现
    }else{
        // 2. 进行登出操作，并移除
        m_single_sessions_online.remove(client_id);
        single_session->logoutSession();
        handleFriendListUpdate(client_id,false);    // 用户下线
    }
    // 3. 发送信息
    SettingLogoutMessage msg(true);
    if(network_tcp->send(msg.getJsonObejct())){
        return true;
    }else{
        errorOccurred("cannot Server handleSettingLogout");
        return false;
    }
}

/*
 * 5. 处理不在线的信息发送
 */
bool Server::handleChatForwardOff(const QJsonObject &json){

    // NOTICE 传入的QJsonObject是ChatForward的
    // 1. 解析传入的json
    const QString& sender_id = json[toString(JsonChatForward::SenderId)].toString();
    const QString& receiver_id = json[toString(JsonChatForward::ReceiverId)].toString();
    // 1. 1 解析传入json的dialog
    const QJsonObject& dialog = json[toString(JsonChatForward::Dialog)].toObject();
    const QString& dialog_time = dialog[toString(JsonDialog::DialogTime)].toString();
    const QString& dialog_content = dialog[toString(JsonDialog::DiaLogContent)].toString();
    const QString& dialog_sender_id = dialog[toString(JsonDialog::DialogSenderId)].toString();

    // 2. 构造条件
    QString condition = toString(DatabaseFriendList::ClientId) + " = '" + receiver_id + "' and " + toString(DatabaseFriendList::FriendId) + " = '" + sender_id + "'";
    const QMap<QString,QVariant>& record = m_db_mysql->selectRecord(toString(DatabaseTableName::FriendList),condition);
    int unread_msg = record[toString(DatabaseFriendList::UnreadMsg)].toInt() + 1;   // 添加一条未读信息
    // 3. 修改friend_id中对client_id信息：lastMsgTime和UnreadMsg
    QMap<QString,QVariant> update;
    update.insert(toString(DatabaseFriendList::LastMsgTime),dialog_time);
    update.insert(toString(DatabaseFriendList::UnreadMsg),unread_msg);
    m_db_mysql->updateRecord(toString(DatabaseTableName::FriendList),condition,update);
    // 4. 修改client_id中对friend_id的信息：lastMsgTime
    condition = toString(DatabaseFriendList::ClientId) + " = '" + sender_id + "' and " + toString(DatabaseFriendList::FriendId) + " = '" + receiver_id + "'";
    update.remove(toString(DatabaseFriendList::UnreadMsg));
    m_db_mysql->updateRecord(toString(DatabaseTableName::FriendList),condition,update);

    // 3. 存储信息
    condition = toString(DatabaseMessages::SenderId) + " = '" + sender_id + "' and " + toString(DatabaseMessages::ReceiverId) + " = '" + receiver_id + "' and " + toString(DatabaseMessages::ReceiverId) + " = '" + receiver_id + "'";
    QMap<QString,QVariant> msg;
    msg.insert(toString(DatabaseMessages::SenderId),dialog_sender_id);
    msg.insert(toString(DatabaseMessages::ReceiverId),receiver_id);
    msg.insert(toString(DatabaseMessages::Time),dialog_time);
    msg.insert(toString(DatabaseMessages::Content),dialog_content);
    m_db_mysql->insertRecord(toString(DatabaseTableName::Messages),msg);

    return true;
}

/*
 * 6. 处理登录上通知的信息
 */
bool Server::handleFriendListUpdate(const QString& client_id, bool is_online){
    // 1. 查看登录的用户好友
    QString condition = toString(DatabaseFriendList::ClientId) + " = '" + client_id + "'";
    const QList<QMap<QString,QVariant>>& res = m_db_mysql->selectRecords(toString(DatabaseTableName::FriendList),condition);    // 获取所有好友id
    for(auto iter = res.begin();iter!=res.end();iter++){
        // 2. 遍历每一个好友，如果在线，则发送信息
        const QString& single_session_id = (iter->value(toString(DatabaseFriendList::FriendId))).toString();
        SingleSession* single_session = m_single_sessions_online.value(single_session_id);
        if(single_session!=nullptr){
            // 3. 存在该用户在线，发送信息
            FriendListUpdateMessage msg(is_online,client_id);
            single_session -> FriendListUpdate(msg.getJsonObejct());
        }else{
            continue;
        }
    }
    return true;
}

/*
 * 7. 处理setting上修改的通知
 */
bool Server::handleFriendListSetting(const QString &client_id, const QString &client_nickname, const QString &client_email, const QString &client_signature){

    // 1. 寻找client_id的好友
    QString condition = toString(DatabaseFriendList::ClientId) + " = '" + client_id + "'";
    const QList<QMap<QString,QVariant>>& res = m_db_mysql->selectRecords(toString(DatabaseTableName::FriendList),condition);    // 获取所有好友id
    for(auto iter = res.begin();iter!=res.end();iter++){
        // 2. 遍历每一个好友
        const QString& single_session_id = (iter->value(toString(DatabaseFriendList::FriendId))).toString();
        SingleSession* single_session = m_single_sessions_online.value(single_session_id);
        if(single_session!=nullptr){
            // 3. 用户在线，发送更改消息
            FriendListSettingMessage msg(client_id,client_nickname,client_email,client_signature);
            single_session -> FriendListSetting(msg.getJsonObejct());
        }
    }
    return true;
}

/*
 * 错误处理
 */
void Server::errorOccurred(const QString &error) const{
    qDebug() << error;
}
