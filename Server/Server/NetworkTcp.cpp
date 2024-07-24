#include "NetworkTcp.h"

/* * * * * * 1. NetworkTcp对象初始化和析构 * * * * * */

/*
 * 1. 构造函数
 */
NetworkTcp::NetworkTcp(QTcpSocket* tcp_socket, QObject *parent)
    : INetworkTcp(parent), m_tcp_socket(tcp_socket){

    // connect
    connect(m_tcp_socket,&QTcpSocket::readyRead,this,&NetworkTcp::do_readyRead);    // socket接受到信息后触发对应NetworkTcp的函数
    connect(m_tcp_socket,&QTcpSocket::disconnected,this,&NetworkTcp::do_disconnected);  // socket断开连接后触发NetworkTcp函数
}

/*
 * 2. 析构函数
 */
NetworkTcp::~NetworkTcp(){
    qDebug() << "~NetworkTcp called";
}





/* * * * * * 2. public接口函数实现 * * * * * */

/*
 * 1. 发送信息
 * 参数：const QJsonObject& json, QTcpSocket* client
 * 返回值：void
 */
bool NetworkTcp::send(const QJsonObject &json) const {
    if(m_tcp_socket && m_tcp_socket->isOpen()){ // 如果用户存在且在线
        QJsonDocument doc{json};
        QByteArray data = doc.toJson(); // 将QJsonObject转换为QByteArray
        m_tcp_socket -> write(data);  // 将QByteArray发送到客户端
        m_tcp_socket -> flush();  // 刷新缓冲区，强制所有信息输出
        return true;
    }else{
        errorOccurred("cannot NetworkTcp Send(), m_tcp_socket not open/exist");
        return false;
    }
}


/* * * * * * 3. private slots 槽函数实现 * * * * * */

/*
 * 1. 接受信息
 * 参数：无
 * 返回值：void
 * 假设：
 */
void NetworkTcp::do_readyRead(){

    // 直接使用自己的socket就可以了
    if(m_tcp_socket && m_tcp_socket->isOpen()){
        QByteArray data;
        while(m_tcp_socket->bytesAvailable()>0){    // 如果有数据可读
            data.resize(m_tcp_socket->bytesAvailable()); // 重新设置data大小
            m_tcp_socket->read(data.data(),data.size()); // 读取数据

            QJsonParseError error;
            QJsonDocument doc = QJsonDocument::fromJson(data,&error); // 将QByteArray转换为QJsonDocument
            if(error.error == QJsonParseError::NoError){ // 如果没有错误
                QJsonObject json = doc.object(); // 将QJsonDocument转换为QJsonObject
                emit received(json);
            }else{
                errorOccurred("cannot NetworkTcp do_readyRead(), json parse error: " + error.errorString());   // 输出错误信息
            }
        }
    }else{
        errorOccurred("cannot NetworkTcp do_readyRead(), no m_tcp_socket connected");
    }
}

/*
 * 2. 处理断开连接
 */
void NetworkTcp::do_disconnected(){
    // 1. 单纯进行封装起来
    emit disconnectNetworkTcp();
}


/*
 * 错误处理函数
 */
void NetworkTcp::errorOccurred(const QString &error) const{
    qDebug() << error;
}
