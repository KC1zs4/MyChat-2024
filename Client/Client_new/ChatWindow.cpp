#include "ChatWindow.h"
#include <QVBoxLayout>

/* * * * * * * 1. 对象初始化和析构函数 * * * * * */

/*
 * 1. 构造
 */
ChatWindow::ChatWindow(const QString &friend_id, const QString& friend_nickname, INetworkTcp* network_tcp, QWidget *parent)
    : QWidget(parent),
    m_friend_id(friend_id), m_friend_nickname(friend_nickname),
    text_input(new QTextEdit(this)), pbtn_send(new QPushButton("发送",this)),
    label_identity(new QLabel(this)), label_email(new QLabel(this)), label_signature(new QLabel(this)),
    m_chat_delegate(new ChatDelegate(friend_id,friend_nickname,this)),
    m_chat_model(new ChatModel(friend_id,this)),m_chat_view(new QListView(this)),
    m_network_tcp(network_tcp){

    this->setAttribute(Qt::WA_DeleteOnClose);

    // 发送信息按键
    connect(pbtn_send,&QPushButton::clicked,this,&ChatWindow::pbtn_sendClicked);

    // 1. 初始化完毕，加载外部消息
    ChatLoadMessage msg(m_friend_id,User::getInstance()->getId());
    if(!(m_network_tcp->send(msg.getJsonObejct()))){
        errorOccurred("cannot ChatModel laodOnlinedialog");
    }
}


/*
 * 2. 析构
 */
ChatWindow::~ChatWindow(){
    emit chatWindowClosed(m_friend_id);
    // 给服务端发送信息，告知已经关闭了
    ChatCloseMessage msg(User::getInstance()->getId(),m_friend_id);
    if(!(m_network_tcp->send(msg.getJsonObejct()))){
        errorOccurred("cannnot ChatWindow ~ChatWindow, send ChatCloseMessage");
    }
    qDebug() << "~ChatWindow called";
}

/* * * * * * 2. 公有接口函数 * * * * * */

/*
 * 1. 处理加载消息
 */
bool ChatWindow::chatLoad(const QJsonObject &json){
    const QJsonObject& info = json[toString(JsonChatLoad::Info)].toObject();
    m_friend_email = info[toString(JsonInfo::ClientEmail)].toString();
    m_friend_signature = info[toString(JsonInfo::ClientSignature)].toString();
    if(m_chat_model->handleChatLoad(json)){
        initialUi();    // 消息加载成功后
        return true;
    }else{
        return false;
    }
}

/*
 * 2. 处理转发消息
 */
bool ChatWindow::chatForward(const QJsonObject &json){

    // NOTICE 此处的json是传入的forward信息
    // 1. 获取Dialog对象
    const QJsonObject& dialog = json[toString(JsonChatForward::Dialog)].toObject();
    const QString& sender_id = dialog[toString(JsonDialog::DialogSenderId)].toString();
    const QString& time = dialog[toString(JsonDialog::DialogTime)].toString();
    const QString& content = dialog[toString(JsonDialog::DiaLogContent)].toString();

    Dialog dia(time,sender_id,content);
    m_chat_model->addDialog(dia);
    m_chat_view->scrollToBottom();
    return true;
}



/* * * * * * 3. private函数 * * * * * */

void ChatWindow::initialUi(){
    resize(800,600);

    m_chat_view->setModel(m_chat_model);
    m_chat_view->setItemDelegate(m_chat_delegate);

    // 1. 视图设置
    m_chat_view->setModel(m_chat_model);
    m_chat_view->setItemDelegate(m_chat_delegate);

    // 2. 分隔交互部分
    label_identity -> setText("好友id：" + m_friend_id + "    好友昵称：" + m_friend_nickname);
    label_email -> setText("好友邮箱：" + m_friend_email);
    label_signature -> setText("好友签名：" + m_friend_signature);

    // 2. 2 先纵向
    QWidget* widget_left_interact = new QWidget(this);
    QVBoxLayout* layout_send_info = new QVBoxLayout(widget_left_interact);
    layout_send_info->addWidget(label_identity);
    layout_send_info->addWidget(label_email);
    layout_send_info->addWidget(label_signature);
    widget_left_interact->setLayout(layout_send_info);

    // 2. 3 再横向
    QWidget* widget_interact = new QWidget(this);
    QHBoxLayout* layout_send = new QHBoxLayout(widget_interact);
    layout_send->addWidget(widget_left_interact);
    layout_send->addWidget(pbtn_send);
    widget_interact->setLayout(layout_send);

    // 3. 纵向布局
    QVBoxLayout* layout_main = new QVBoxLayout(this);

    layout_main->addWidget(m_chat_view);
    layout_main->addWidget(widget_interact);
    layout_main->addWidget(text_input);
    this->setLayout(layout_main);


    // 4. 将视图滚动至最下端
    m_chat_view->scrollToBottom();
    this->show();
}

/* * * * * * 3. private slots槽函数 * * * * * */

/*
 * 1. 发送信息按钮
 */
void ChatWindow::pbtn_sendClicked(){

    const QString& content = text_input -> toPlainText();
    text_input->clear();
    if(content.isEmpty()){
        QMessageBox::information(this,"信息发送","信息不能为空");
        return;
    }else{

        // 1. 发送消息
        QJsonObject dialog;
        const QString& time = QDateTime::currentDateTime().toString(Qt::ISODate);
        dialog[toString(JsonDialog::DialogTime)] = time;  // 获取当前时间
        dialog[toString(JsonDialog::DialogSenderId)] = User::getInstance()->getId();
        dialog[toString(JsonDialog::DiaLogContent)] = content;
        ChatForwardMessage msg(User::getInstance()->getId(),m_friend_id,dialog);
        if(!(m_network_tcp->send(msg.getJsonObejct()))){
            errorOccurred("cannot ChatWindow pbtn_sendClicked");
            return;
        }
        // 2. 发送成功才发送到视图中
        Dialog dia(QDateTime::currentDateTime().toString(Qt::ISODate),User::getInstance()->getId(),content);
        m_chat_model->addDialog(dia);
        m_chat_view->scrollToBottom();
        // 3. 发出信号提醒好友列表FriendListWinow进行修改
        emit chatWindowSendDialog(m_friend_id,time);
    }
}

/*
 * 2. 响应FriendListWindow关闭
 */
void ChatWindow::do_friendListClosed(){
    this->close();
}


void ChatWindow::errorOccurred(const QString& error){
    qDebug() << error;
}



