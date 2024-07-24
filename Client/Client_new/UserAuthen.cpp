#include "UserAuthen.h"

/* * * * * * 1. 对象初始化和析构 * * * * * */

/*
 * 1. 构造函数
 */
UserAuthen::UserAuthen(INetworkTcp *network_tcp, QObject *parent)
    : IUserAuthen(parent), m_network_tcp(network_tcp){

    connect(m_network_tcp,&INetworkTcp::userAuthenAffair,this,&UserAuthen::do_userAuthenAffair);
}

/*
 * 2. 析构函数
 */
UserAuthen::~UserAuthen(){
    qDebug() << "~UserAuthen called";
}


/* * * * * 2. 接口的实现 * * * * * */

/*
 * 1. 用户登录
 */
bool UserAuthen::loginUser(const QString &client_id, const QString &client_password, const QString &host, quint16 port){
    if(!(m_network_tcp->getSocketState() == QAbstractSocket::ConnectedState)){
        if(!m_network_tcp->connectToHost(host,port)){
            errorOccurred("cannot UserAuthen loginUser, connectToHost error");
            return false;
        }
        // 成功连接到对方
    }

    LoginMessage msg(client_id,client_password);
    if(m_network_tcp->send(msg.getJsonObejct())){
        return true;
    }else{
        errorOccurred("cannot UserAuthen loginUser, sendJson error");
        return false;
    }
}

/*
 * 2. 用户注册
 */
bool UserAuthen::registerUser(const QString &client_id, const QString &client_password, const QString &host, quint16 port){
    if(!(m_network_tcp->getSocketState() == QAbstractSocket::ConnectedState)){
        if(!m_network_tcp->connectToHost(host,port)){
            errorOccurred("cannot UserAuthen RegisterUser, connectToHost error");
            return false;
        }
    }
    RegisterMessage msg(client_id,client_password);
    if(m_network_tcp->send(msg.getJsonObejct())){
        return true;
    }else{
        errorOccurred("cannot UserAuthen registerUser, sendJson error");
        return false;
    }
}





/* * * * * * 3. private slots 槽函数 * * * * * */

/*
 * 1. 处理Userauthen信息
 */
void UserAuthen::do_userAuthenAffair(const QJsonObject &json){

    if(json[toString(JsonKey::Action)].toString() == toString(JsonAction::Login)){
        const QJsonObject& login = json[toString(JsonKey::Login)].toObject();
        emit loginAffair(login,m_network_tcp);
    }else if(json[toString(JsonKey::Action)].toString() == toString(JsonAction::Register)){
        const QJsonObject& registr = json[toString(JsonKey::Register)].toObject();
        emit registerAffair(registr);
    }
}

/*
 * 错误处理函数
 */
void UserAuthen::errorOccurred(const QString &error){
    qDebug() << error;
}
