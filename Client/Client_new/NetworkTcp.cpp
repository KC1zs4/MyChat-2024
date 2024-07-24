#include "NetworkTcp.h"
#include <QMessageBox>

/* * * * * * 1. 构造函数和析构函数 * * * * * */



/*
 * 1. 构造函数
 */
NetworkTcp::NetworkTcp(QObject *parent)
    : INetworkTcp(parent),m_tcp_socket{new QTcpSocket(this)}{

    // 进行初始化连接
    // connect(m_tcp_socket,&QTcpSocket::connected,this,&NetworkTcp::do_connected);
    connect(m_tcp_socket,&QTcpSocket::disconnected,this,&NetworkTcp::do_disconnected);
    connect(m_tcp_socket,&QTcpSocket::readyRead,this,&NetworkTcp::do_readyRead);
}

/*
 * 2. 析构函数
 * 改进：
 *      1. 需要检查是否连接着——false
 */
NetworkTcp::~NetworkTcp(){
    m_tcp_socket->disconnectFromHost();  // 断开连接
    m_tcp_socket->close();  // 关闭socket，释放资源
    qDebug() << "~NetworkTcp called";
}



/* * * * * * 2. public接口实现 * * * * * */

/*
 * 1. 连接函数
 * 改进：
 *      1. 错误处理暂时统一实现
 */
bool NetworkTcp::connectToHost(const QString &host, quint16 port){
    m_tcp_socket->connectToHost(host,port);
    if(m_tcp_socket->waitForConnected(2000)){
        return true;
    }else{
        QMessageBox::information(nullptr,"网络连接","连接超时，请重试");
        // errorOccurred("cannot NetworkTcp connectToHost");
        return false;
    }
}

/*
 * 2. 断开连接
 * 改进：
 *      1. 错误处理没有实现
 */
bool NetworkTcp::disconnectFromHost(){
    if(m_tcp_socket->state() == QAbstractSocket::ConnectedState){
        m_tcp_socket->disconnectFromHost();
        if(m_tcp_socket->waitForDisconnected(2000)){
            return true;
        }else{
            errorOccurred("cannot NetworkTcp disconnectFromHost");
            return false;
        }
    }else{
        errorOccurred("cannot NetworkTcp disconnectFromHost, already not connected");
        return true;
    }
}

/*
 * 3. json数据发送
 */
bool NetworkTcp::send(const QJsonObject &json){
    QJsonDocument doc(json);
    QByteArray byte_array = doc.toJson(QJsonDocument::Compact);
    if(m_tcp_socket->write(byte_array) == byte_array.size()){
        return true;
    }else{
        errorOccurred("cannot NetworkTcp send");
        return false;
    }
}

/*
 * 4. 获取连接状态
 */
QAbstractSocket::SocketState NetworkTcp::getSocketState(){
    return m_tcp_socket->state();
}





/* * * * * * 3. private slots槽函数实现 * * * * * */


/*
 * 1. do_disconnected
 * 改进：
 *      1. 对断开连接进行处理——应该回到登录界面
 */
void NetworkTcp::do_disconnected(){
    QMessageBox::information(nullptr,"网络连接","断开连接");
    emit disconnectedNetworkTcp();
}

/*
 * 2. 接受信息并转发
 */
void NetworkTcp::do_readyRead(){
    QByteArray data;
    while(m_tcp_socket->bytesAvailable()>0){
        data.resize(m_tcp_socket->bytesAvailable());
        m_tcp_socket->read(data.data(),data.size());
    }
    QJsonParseError parse_error;
    QJsonDocument doc = QJsonDocument::fromJson(data,&parse_error);
    if (!parse_error.error){

        if(doc.isObject()){
            // 消息解析正确
            QJsonObject json = doc.object();

            if(json[toString(JsonKey::Type)].toString() == toString(JsonType::UserAuthen)){
                emit userAuthenAffair(json);
            }else if(json[toString(JsonKey::Type)].toString() == toString(JsonType::Friend)){
                emit friendAffair(json);
            }else if(json[toString(JsonKey::Type)].toString() == toString(JsonType::Chat)){
                emit chatAffair(json);
            }else if(json[toString(JsonKey::Type)].toString() == toString(JsonType::Setting)){
                emit settingAffair(json);
            }
        }else{
            errorOccurred("cannot NetworkTcp do_readyRead, Received JSON is not an object");
        }
    }else{
        errorOccurred("cannot NetworkTcp do_readyRead, JSON parse error");
    }
}


/*
 * 错误函数
 */
void NetworkTcp::errorOccurred(const QString &error){
    qDebug() << error;
}
