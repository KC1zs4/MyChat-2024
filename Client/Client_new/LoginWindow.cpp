#include "LoginWindow.h"
#include "ui_LoginWindow.h"
#include <QMessageBox>
#include <QRegularExpression>

/* * * * * * 1. LoginWindow对象及其初始化 * * * * * */

/*
 * 1. 构造函数
 */
LoginWindow::LoginWindow(QWidget *parent)
    : QWidget(parent), ui(new Ui::LoginWindow), m_user_authen(new UserAuthen(new NetworkTcp(this),this)){

    this->setAttribute(Qt::WA_DeleteOnClose);   // 关闭窗口时调用析构函数
    ui->setupUi(this); 

    // 初始化连接
    ui->line_host->setPlaceholderText("服务器IP");
    ui->line_host->setText("127.0.0.1");
    ui->line_host->setReadOnly(true);    // 将line_host设置为只读
    ui->spin_port->setValue(8080);  // 自身有设定范围为0-10000

    // 控件连接
    connect(ui->pbtn_login,&QPushButton::clicked,this,&LoginWindow::pbtn_loginFunc);
    connect(ui->pbtn_register,&QPushButton::clicked,this,&LoginWindow::pbtn_registerFunc);

    // UserAuthen连接
    connect(m_user_authen,&IUserAuthen::loginAffair,this,&LoginWindow::do_loginAffair);
    connect(m_user_authen,&IUserAuthen::registerAffair,this,&LoginWindow::do_registerAffair);

    this->setWindowTitle("登录界面");
    // this->show();
}


/*
 * 2. 析构函数
 */
LoginWindow::~LoginWindow(){
    delete ui;  // 这个需要手动解决
    qDebug() << "~LoginWindow called";
}



/* * * * * * 2. 槽函数实现 * * * * * * */

/*
 * 1. 登录按钮按下
 * 改进：
 *      1. 是否需要保证不能一次按下多次——无需，会进行冻结
 */
void LoginWindow::pbtn_loginFunc(){
    const QString& id = ui -> line_id -> text();
    const QString& password = ui -> line_password -> text();
    const QString& host = ui->line_host -> text();
    quint16 port = ui->spin_port -> value();
    if(isValid(id,password)){
        m_user_authen -> loginUser(id,password,host,port);
    }
}

/*
 * 2. 注册按钮按下
 */
void LoginWindow::pbtn_registerFunc(){
    const QString& id = ui -> line_id -> text();
    const QString& password = ui -> line_password -> text();
    const QString& host = ui->line_host -> text();
    quint16 port = ui->spin_port -> value();
    if(isValid(id,password)){
       m_user_authen -> registerUser(id,password,host,port);
    }
}


/*
 * 3. 登录信息处理
 */
void LoginWindow::do_loginAffair(const QJsonObject &json, INetworkTcp* network_tcp){

    if(json[toString(JsonLogin::Success)].toBool()){
        const QJsonObject& info = json[toString(JsonLogin::Info)].toObject();
        User::getInstance()->setId(info[toString(JsonInfo::ClientId)].toString());
        User::getInstance()->setNickname(info[toString(JsonInfo::ClientNickname)].toString());
        User::getInstance()->setEmail(info[toString(JsonInfo::ClientEmail)].toString());
        User::getInstance()->setSignature(info[toString(JsonInfo::ClientSignature)].toString());

        this->hide();   // 隐藏自身窗口，可用于后续退出重新登录
        // 进行friendlist的处理
        // QMessageBox::information(this,"用户登录","登录成功"); 前期未开发好，暂时不显示

        // 获取当前的NetworkTcp
        FriendListWindow* friend_list = new FriendListWindow(network_tcp);  // 不可以设置this，否则登录窗口hide了不会显示，有WA_DeleteOnClose了
        connect(friend_list,&FriendListWindow::loginShow,this,&LoginWindow::do_loginShow);
        connect(friend_list,&FriendListWindow::loginClose,this,&LoginWindow::do_loginClose);

    }else{
        if(json[toString(JsonLogin::Error)].toBool()){
            QMessageBox::information(this,"用户登录","登录失败，服务端错误，请重试");
        }else{
            if(json[toString(JsonLogin::IsOnline)].toBool()){
                QMessageBox::information(this,"用户登录","登录失败，用户已登录");
            }else{
                if(!(json[toString(JsonLogin::Exist)].toBool())){
                    QMessageBox::information(this,"用户登录","登录失败，用户不存在，请先登录");
                }else{
                    QMessageBox::information(this,"用户登录","登录失败，密码错误，请重试");
                }
            }
        }
    }
}


/*
 * 4. 注册信息处理
 */
void LoginWindow::do_registerAffair(const QJsonObject &json){
    if(json[toString(JsonRegister::Success)].toBool()){
        QMessageBox::information(this,"用户注册","注册成功，请登录");
    }else{
        if(json[toString(JsonRegister::Error)].toBool()){
            QMessageBox::information(this,"用户注册","注册失败，服务器错误，请重试");
        }else{
            if(json[toString(JsonRegister::Exist)].toBool()){
                QMessageBox::information(this,"用户注册","注册失败，用户已存在");
            }
        }
    }
}

/*
 * 5. 重新登录展示槽函数
 */
void LoginWindow::do_loginShow(){
    ui->line_id->clear();
    ui->line_password->clear();
    this->show();
}


/*
 * 6. 关闭好友列表时程序终止
 */
void LoginWindow::do_loginClose(){
    this->close();  // 自动调用析构函数，释放资源
}

/*
 * 检查数据合法性
 */
bool LoginWindow::isValid(const QString& client_id, const QString& client_password) {
    // 1. 判断 ID 和密码是否有空白字符
    if (client_id.trimmed().isEmpty()) {
        QMessageBox::information(this, "用户登录", "ID为空，请重新输入");
        return false;
    }
    if (client_password.trimmed().isEmpty()) {
        QMessageBox::information(this, "用户登录", "密码为空，请重新输入");
        return false;
    }

    // 2. 判断 ID 和密码是否包含不允许的特殊字符
    QRegularExpression specialChars("[\\s'#]"); // 正则表达式匹配空格、单引号和井号
    if (specialChars.match(client_id).hasMatch()) {
        QMessageBox::information(this, "用户登录", "ID中不能含有'、#、空格");
        return false;
    }
    if (specialChars.match(client_password).hasMatch()) {
        QMessageBox::information(this, "用户登录", "密码中不能含有'、#、空格");
        return false;
    }

    // 3. 判断 ID 和密码的长度是否合法
    // 假设合法长度范围是 6 到 12 个字符
    if (client_id.length() <= 0 || client_id.length() > 12) {
        QMessageBox::information(this, "用户登录", "ID长度必须在6到12个字符之间");
        return false;
    }
    if (client_password.length() <= 0 || client_password.length() > 12) {
        QMessageBox::information(this, "用户登录", "密码长度必须在6到12个字符之间");
        return false;
    }

    // 如果所有检查都通过，则返回 true
    return true;
}

/*
 * 错误处理
 */
void LoginWindow::errorOccurred(const QString &error){
    qDebug() << error;
}
