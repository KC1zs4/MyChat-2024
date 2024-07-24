#include "UserAuthen.h"

/* * * * * * 1. 构造函数和析构函数 * * * * * */

/*
 * 1. 构造函数
 */
UserAuthen::UserAuthen(IDatabaseMySQL *db_mysql, INetworkTcp *network_tcp, QObject *parent)
    : IUserAuthen{parent}, m_db_mysql{db_mysql}, m_network_tcp{network_tcp}{}

/*
 * 2. 析构函数
 */
UserAuthen::~UserAuthen(){
    qDebug() << "~UserAuthen called";
}


/* * * * * * 2. 接口实现 * * * * * */

/*
 * 1. 用户注册
 */
bool UserAuthen::RegisterUser(const QString &client_id, const QString &client_password){

    QString condition = "Binary " + toString(DatabaseUserInfo::ClientId) + " = '" + client_id + "'";    // 使用Binary关键字使得查询大小写敏感
    QMap<QString,QVariant> data = m_db_mysql->selectRecord(toString(DatabaseTableName::UserInfo),condition);

    if(!data.isEmpty()){
        // 1. 用户存在，返回错误信息
        // NOTICE 由于MySQL没有开启大小写敏感，需要进行直接比对
        RegisterMessage msg(false,true,false);
        if(m_network_tcp->send(msg.getJsonObejct())){
            return true;
        }else{
            errorOccurred("cannot UserAuthen registerUser");
            return true;
        }
    }else{

        // 2. 用户不存在，进行注册
        data[toString(DatabaseUserInfo::ClientId)] = client_id;
        data[toString(DatabaseUserInfo::ClientPassword)] = client_password;
        condition = toString(DatabaseUserInfo::ClientId) + " = '" + client_id + "'";    // 此处无需Binary关键字
        bool success = m_db_mysql->insertRecord(toString(DatabaseTableName::UserInfo),data);
        if(!success){
            // 2. 1 注册失败，返回false
            RegisterMessage msg(false,false,true);  // 出现错误
            if(m_network_tcp->send(msg.getJsonObejct())){
                return false;
            }else{
                errorOccurred("cannot UserAuthen registerUser2");
                return false;
            }
        }else{
            // 2. 2 注册成功，返回true
            RegisterMessage msg(true,false,false);
            if(m_network_tcp->send(msg.getJsonObejct())){
                return true;
            }else{
                errorOccurred("cannot UserAuthen registerUser3");
                return true;
            }
        }
    }
}


/*
 * 2. 用户登录
 */
bool UserAuthen::loginUser(const QString &client_id, const QString &client_password){

    // NOTICE 能到这一步来说明都不在线，在线的直接发送消息回去

    QString condition = "Binary " + toString(DatabaseUserInfo::ClientId) + " = '" + client_id + "'";
    QMap<QString,QVariant> data = m_db_mysql->selectRecord(toString(DatabaseTableName::UserInfo),condition);
    QJsonObject json;
    if(data.isEmpty()){
        // 1. 用户不存在
        LoginMessage msg(false,false,false,false,json);
        if(m_network_tcp->send(msg.getJsonObejct())){
            return false;
        }else{
            errorOccurred("cannot UserAuthen loginUser");
            return false;
        }
    }else{
        // 2. 用户存在

        // 2. 1 如果用户密码正确
        if(data[toString(DatabaseUserInfo::ClientPassword)] == client_password){
            for(auto it = data.begin(); it != data.end(); ++it){
                json[it.key()] = it.value().toString();
            }
            LoginMessage msg(true,true,false,false,json);
            if(m_network_tcp->send(msg.getJsonObejct())){

                // 成功发送信息的话更改数据库状态
                QMap<QString,QVariant> login;
                login.insert(toString(DatabaseUserInfo::IsOnline),1);
                m_db_mysql->updateRecord(toString(DatabaseTableName::UserInfo),condition,login);

                return true;
            }else{
                errorOccurred("cannot UserAuthen loginUser2");
                return true;
            }
        }else{
            // 2. 2 用户密码错误
            LoginMessage msg(false,true,false,false,json);
            if(m_network_tcp->send(msg.getJsonObejct())){
                return false;
            }else{
                errorOccurred("cannot UserAuthen loginUser3");
                return false;
            }
        }
    }
}

/*
 * 3. 用户退出
 */
bool UserAuthen::logoutUser(const QString& client_id){
    QMap<QString,QVariant> logout;
    logout.insert(toString(DatabaseUserInfo::IsOnline),0);
    // 1. 将所有的用户状态更改为不在线
    QString condition = "BINARY " + toString(DatabaseUserInfo::ClientId) + " = '" + client_id + "'";    // 使用Binary关键字使得查询大小写敏感
    bool success = m_db_mysql->updateRecord(toString(DatabaseTableName::UserInfo),condition,logout);
    if(success){
        // 2. 将该用户所有的窗口is_open设置为0
        QMap<QString,QVariant> is_open;
        is_open.insert(toString(DatabaseFriendList::IsOpen),0);
        success = m_db_mysql->updateRecord(toString(DatabaseTableName::FriendList),condition,is_open);
        if(success){
            return false;
        }else{
            return true;
        }
    }else{
        errorOccurred("cannot logoutUser");
        return false;
    }
}


/*
 * 错误处理函数
 */
void UserAuthen::errorOccurred(const QString &error) const{
    qDebug() << error;
}
