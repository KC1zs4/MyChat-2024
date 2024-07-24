#include "SettingWindow.h"
#include "ui_SettingWindow.h"
#include "User.h"

/* * * * * * 1. setting窗口初始化和析构 * * * * * */

/*
 * 1. 构造函数
 */
SettingWindow::SettingWindow(INetworkTcp *network_tcp, QWidget *parent)
    : QWidget(parent), ui(new Ui::SettingWindow), m_network_tcp(network_tcp){

    this->setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);

    ui->line_nickname->setPlaceholderText(User::getInstance()->getNickname());
    ui->line_email->setPlaceholderText(User::getInstance()->getEmail());
    ui->line_signature->setPlaceholderText(User::getInstance()->getSignature());

    // 信号槽连接
    connect(ui->pbtn_return,&QPushButton::clicked,this,&SettingWindow::pbtn_returnClicked);
    connect(ui->pbtn_save,&QPushButton::clicked,this,&SettingWindow::pbtn_saveClicked);
    connect(ui->pbtn_logout,&QPushButton::clicked,this,&SettingWindow::pbtn_logoutClicked);
    connect(m_network_tcp,&INetworkTcp::settingAffair,this,&SettingWindow::do_settingAffair);
}

/*
 * 2. 析构函数
 */
SettingWindow::~SettingWindow(){
    delete ui;
    qDebug() << "~SettingWindow called";
}




/* * * * * * 2. setting窗口槽函数 * * * * * */

/*
 * 1. 返回函数
 */
void SettingWindow::pbtn_returnClicked(){
    this->close();
}

/*
 * 2. 保存函数
 */
void SettingWindow::pbtn_saveClicked(){

    // 1. 获取新信息
    QString client_nickname = ui->line_nickname->text();
    QString client_email = ui->line_email->text();
    QString client_signature = ui->line_signature->text();

    if(client_nickname.isEmpty()){
        client_nickname = User::getInstance()->getNickname();
    }else{
        if(!isValid(client_nickname)){
            return;
        }
    }

    if(client_email.isEmpty()){
        client_email = User::getInstance()->getEmail();
    }else{
        if(!isValid(client_email)){
            return;
        }
    }

    if(client_signature.isEmpty()){
        client_signature = User::getInstance()->getSignature();
    }else{
        if(!isValid(client_signature)){
            return;
        }
    }

    // 2. 构造信息
    SettingSaveMessage msg(User::getInstance()->getId(),client_nickname,client_email,client_signature);

    // 3. 发送信息
    if(!(m_network_tcp->send(msg.getJsonObejct()))){
        errorOccurred("cannot SettingWindow pbtn_saveClicked");
    }
}


/*
 * 3. 退出函数
 */
void SettingWindow::pbtn_logoutClicked(){

    // 1. 构造消息
    SettingLogoutMessage msg(User::getInstance()->getId());
    // 2. 发送消息
    if(!(m_network_tcp->send(msg.getJsonObejct()))){
        errorOccurred("cannot SettingWindow pbtn_logoutClicked");
    }
}


/*
 * 4. 处理接受到的消息
 * 功能：其实就只有一个，就是保存资料
 */
void SettingWindow::do_settingAffair(const QJsonObject &json){

    // 1. 判断消息种类
    if(json[toString(JsonKey::Action)].toString() == toString(JsonAction::Save)){

        const QJsonObject& save = json[toString(JsonKey::SettingSave)].toObject();
        handleSettingSave(save);

    }else if(json[toString(JsonKey::Action)].toString() == toString(JsonAction::Logout)){

        const QJsonObject& logout = json[toString(JsonKey::SettingLogout)].toObject();
        handleSettingLogout(logout);
    }
}


/*
 * 5. 响应FriendListWindow中的关闭程序
 */
void SettingWindow::do_friendListClosed(){
    this->close();      // 内存有WA_DeleteOnClose了
}


/* * * * * * 3. 私有辅助函数 * * * * * */

void SettingWindow::handleSettingSave(const QJsonObject &json){

    if(json[toString(JsonSettingSave::Success)].toBool()){
        User::getInstance()->setNickname(json[toString(JsonSettingSave::ClientNickname)].toString());
        User::getInstance()->setEmail(json[toString(JsonSettingSave::ClientEmail)].toString());
        User::getInstance()->setSignature(json[toString(JsonSettingSave::ClientSignature)].toString());

        // 更新显示
        ui->line_nickname->setPlaceholderText(User::getInstance()->getNickname());
        ui->line_email->setPlaceholderText(User::getInstance()->getEmail());
        ui->line_signature->setPlaceholderText(User::getInstance()->getSignature());

        QMessageBox::information(this,"用户设置","设置成功");
    }else{
        QMessageBox::information(this,"用户设置","设置失败，请重试");
    }
}


void SettingWindow::handleSettingLogout(const QJsonObject &json){

    if(json[toString(JsonSettingLogout::Success)].toBool()){
        emit logoutUser();
    }
}

/*
 * 输入有效性检测
 */
bool SettingWindow::isValid(const QString & str){

    // 2. 判断 ID 和密码是否包含不允许的特殊字符
    QRegularExpression specialChars("[\\s'#]"); // 正则表达式匹配空格、单引号和井号
    if (specialChars.match(str).hasMatch()) {
        QMessageBox::information(this, "用户设置", "用户信息中不能含有'、#、空格");
        return false;
    }


    // 3. 判断 ID 和密码的长度是否合法
    // 假设合法长度范围是 6 到 12 个字符
    if (str.length() <= 0 || str.length() > 12) {
        QMessageBox::information(this, "用户设置", "用户信息中不能含有'、#、空格");
        return false;
    }

    // 如果所有检查都通过，则返回 true
    return true;
}

void SettingWindow::errorOccurred(const QString& error){
    qDebug() << error;
}
