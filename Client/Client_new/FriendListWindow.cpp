#include "FriendListWindow.h"
#include <QRegularExpression>

/* * * * * * 1. 对象初始化和销毁 * * * * * */

/*
 * 1. 构造函数
 * 改进：
 *      1. 假设信号的发射总是成功的
 */
FriendListWindow::FriendListWindow(INetworkTcp* network_tcp, QWidget *parent)
    : QWidget(parent),
    m_network_tcp(network_tcp),
    m_list_model(new FriendListModel(this)), m_list_proxy_model(new FriendListProxyModel(this)),
    m_list_delegate(new FriendListDelegate(this)), m_list_view(new QListView(this)),
    m_exit_user(0){

    this->setAttribute(Qt::WA_DeleteOnClose);   // 在关闭窗口时析构对象

    // 1. 确定窗口大小
    resize(300,700);

    // 2. 初始化控件
    line_search = new QLineEdit(this);
    line_search->setPlaceholderText("请输入ID进行查找");
    pbtn_add_friend = new QPushButton("添加好友", this);
    pbtn_delete_friend = new QPushButton("删除好友", this);
    pbtn_sort = new QPushButton("排序", this);
    pbtn_setting = new QPushButton("设置",this);


    // 设置sort的下拉框
    combox_sort_type = new QComboBox(this);
    combox_sort_type->addItem("默认");
    combox_sort_type->addItem("好友ID");
    combox_sort_type->addItem("好友昵称");
    combox_sort_type->addItem("未读消息");


    // 设置sort的面板
    QWidget* widget_sort = new QWidget(this);
    QHBoxLayout* layout_sort = new QHBoxLayout(widget_sort);
    layout_sort->addWidget(combox_sort_type);
    layout_sort->addWidget(pbtn_sort);
    layout_sort->addWidget(pbtn_setting);
    widget_sort -> setLayout(layout_sort);
    widget_sort -> setFixedHeight(50);

    // 设置add_delete都面板
    QWidget* widget_add_delete = new QWidget(this);
    QHBoxLayout* layout_add_delete = new QHBoxLayout(widget_add_delete);
    layout_add_delete->addWidget(pbtn_add_friend);
    layout_add_delete->addWidget(pbtn_delete_friend);
    widget_add_delete->setLayout(layout_add_delete);
    widget_add_delete->setFixedHeight(50);

    // 设置search_line的面板
    QWidget* widget_search_line = new QWidget(this);
    QHBoxLayout* layout_search_line = new QHBoxLayout(widget_search_line);
    layout_search_line->addWidget(line_search);
    widget_search_line->setLayout(layout_search_line);
    widget_search_line->setFixedHeight(50);


    // layout main主布局
    QVBoxLayout* layout_main = new QVBoxLayout(this);
    layout_main->addWidget(widget_search_line);
    layout_main->addWidget(widget_add_delete);
    layout_main->addWidget(widget_sort);
    layout_main->addWidget(m_list_view);
    layout_main->setSpacing(0);
    layout_main->setContentsMargins(0,0,0,0);

    setWindowTitle("好友列表");

    // 3. 视图模型初始化
    m_list_proxy_model->setSourceModel(m_list_model);
    m_list_proxy_model->setDynamicSortFilter(true);
    m_list_view->setModel(m_list_proxy_model);
    m_list_view->setItemDelegate(m_list_delegate);

    // 4. 连接信号
    // 4. 1. network与friendlist进行连接
    connect(network_tcp,&INetworkTcp::friendAffair,this,&FriendListWindow::do_friendAffair);
    connect(network_tcp,&INetworkTcp::chatAffair,this,&FriendListWindow::do_chatAffair);
    // 4. 2 自身控件的连接
    connect(pbtn_sort,&QPushButton::clicked,this,&FriendListWindow::pbtn_sortClicked);
    connect(pbtn_add_friend,&QPushButton::clicked,this,&FriendListWindow::pbtn_addFriendClicked);
    connect(pbtn_delete_friend,&QPushButton::clicked,this,&FriendListWindow::pbtn_deleteFriendClicked);
    connect(pbtn_setting,&QPushButton::clicked,this,&FriendListWindow::pbtn_settingClicked);
    // 4. 3 视图的连接
    connect(m_list_view,&QListView::clicked,this,&FriendListWindow::do_itemClicked);


    // 5. 发送更新信号
    friendListWindowLoad();
}


/*
 * 2. 析构函数
 */
FriendListWindow::~FriendListWindow(){

    if(!m_exit_user){
        emit loginClose();  // 程序关闭
    }
    emit friendListClosed();
    qDebug() << "~FriendListWindow called";
}

/* * * * * * 2. 私有函数 * * * * * */

/*
 * 1. 发送加载信息
 */
bool FriendListWindow::friendListWindowLoad(){

    // 1. 构造信息
    const QString& client_id = User::getInstance()->getId();
    FriendListLoadMessage msg(client_id);

    // 2. 进行发送
    if(m_network_tcp->send(msg.getJsonObejct())){
        return true;
    }else{
        // errorOccurred("cannot FriendListWindow friendListWindowLoad, cannot send");
        return false;
    }

}

/*
 * 2. 发送添加信息
 */
bool FriendListWindow::friendListWindowAdd(){

    // 1. 构造信息
    const QString& client_id = User::getInstance()->getId();
    const QString& client_nickname = User::getInstance()->getNickname();
    const QString& friend_id = line_search->text();

    // 2. 检查是否已经存在该用户
    if(m_list_model->isContainFriend(friend_id)){
        QMessageBox::information(this,"好友添加","好友已存在");
        return true;
    }else{
        FriendListAddMessage msg(client_id,client_nickname,friend_id);
        if(m_network_tcp->send(msg.getJsonObejct())){
            return true;
        }else{
            // errorOccurred("cannot FriendListWindow::friendListWindowAdd()");
            return false;
        }
    }
}

/*
 * 3. 发送删除信息
 */
bool FriendListWindow::friendListWindowDelete(){

    // 1. 判断是否为空
    const QString& client_id = User::getInstance()->getId();
    const QString& friend_id = line_search->text();
    if(friend_id.isEmpty()){
        QMessageBox::information(this,"好友删除","空内容，请重试");
        return false;
    }else{
        // 2. 判断是否存在
        if(!(m_list_model->isContainFriend(friend_id))){
            QMessageBox::information(this,"好友删除","找不到好友");
            return false;
        }else{
            // 3. 构造信息
            FriendListDeleteMessage msg(client_id,friend_id);
            if(m_network_tcp->send(msg.getJsonObejct())){
                return true;
            }else{
                // errorOccurred("cannot FriendListWindow::friendListWindowDelete()");
                return false;
            }
        }
    }

}

/*
 * 4. 更新消息
 */
bool FriendListWindow::chatUpdate(const QJsonObject &json){
    return m_list_model->handleChatUpdate(json);
}

/*
 * 排序更新
 */
bool FriendListWindow::handleSort(){
    int column = 0; // 默认
    Qt::SortOrder order = Qt::DescendingOrder;
    QString sort_choice = combox_sort_type->currentText();
    if(sort_choice == "好友ID"){
        column = 1;
        order = Qt::AscendingOrder;
    }else if(sort_choice == "好友昵称"){
        column = 2;
        order = Qt::AscendingOrder;
    }else if(sort_choice == "未读消息"){
        column = 3;
    }

    m_list_proxy_model->sort(column,order);
    m_list_view->update();
    return true;
}

/* * * * * * 3. private slots槽函数 * * * * * */

/*
 * 1. 处理接受信息
 */
void FriendListWindow::do_friendAffair(const QJsonObject &json){

    // 1. 对文件信息进行分类
    const QString& action = json[toString(JsonKey::Action)].toString();
    if(action == toString(JsonAction::Load)){

        const QJsonObject& load = json[toString(JsonKey::FriendListLoad)].toObject();
        if(m_list_model->handleFriendListLoad(load)){
            // 1. 1 加载时首次打开，进行排序
            // NOTICE 一般都是会成功的
            m_list_proxy_model->sort(0,Qt::DescendingOrder);    // 默认排序是降序
            this->show();   // 将窗口展示出来
        }

    }else if(action == toString(JsonAction::Add)){

        const QJsonObject& add = json[toString(JsonKey::FriendListAdd)].toObject();
        if(m_list_model->handleFriendListAdd(add)){
            handleSort();   // 按照所有排序进行排序
            QMessageBox::information(this,"好友添加","成功添加好友");
        }else{
            QMessageBox::information(this,"好友添加","不存在该id用户");
        }


    }else if(action == toString(JsonAction::Delete)){

        const QJsonObject& del = json[toString(JsonKey::FriendListDetete)].toObject();
        if(m_list_model->handleFriendListDelete(del)){
            handleSort();
            QMessageBox::information(this,"好友删除","成功删除好友");
        }else{
            QMessageBox::information(this,"好友删除","好友删除失败,请重试");
        }
    }else if(action == toString(JsonAction::Update)){

        const QJsonObject& update = json[toString(JsonKey::FriendListUpdate)].toObject();
        if(!(m_list_model->handleFriendListUpdate(update))){
            // errorOccurred("cannot FriendListWindow Update");
        }
        handleSort();
    }else if(action == toString(JsonAction::Save)){
        const QJsonObject& setting = json[toString(JsonKey::FriendListSetting)].toObject();
        // 更改id即可
        if(!(m_list_model->handleFriendListSetting(setting))){
            // errorOccurred("cannot FriendlistWindow Setting");
        }
        handleSort();
    }
}

/*
 * 2. 处理chat信息
 */
void FriendListWindow::do_chatAffair(const QJsonObject &json){
    if(json[toString(JsonKey::Action)].toString() == toString(JsonAction::Load)){
        const QJsonObject& load = json[toString(JsonKey::ChatLoad)].toObject();
        const QString& friend_id = load[toString(JsonChatLoad::SenderId)].toString();
        ChatWindow* chat = m_chats.value(friend_id);
        if(chat!=nullptr){
            // NOTICE 一般是存在的
            chat->chatLoad(load);
        }
    }else if(json[toString(JsonKey::Action)].toString() == toString(JsonAction::Forward)){

        const QJsonObject& forward = json[toString(JsonKey::ChatForward)].toObject();
        const QString& friend_id = forward[toString(JsonChatForward::SenderId)].toString();
        ChatWindow* chat = m_chats.value(friend_id);
        m_list_model->handleChatWindowTime(friend_id);
        if(chat!=nullptr){
            // NOTICE 一般是存在
            chat->chatForward(forward);
        }
    }else if(json[toString(JsonKey::Action)].toString() == toString(JsonAction::Update)){

        const QJsonObject& update = json[toString(JsonKey::ChatUpdate)].toObject();
        chatUpdate(update); // 比较特殊，是在FriendList中解决的，不需要使用chat
    }
}

/*
 * 2. do_itemClicked()
 */
void FriendListWindow::do_itemClicked(const QModelIndex &index){

    QString friend_id = index.data(IFriendListModel::FriendRole::IdRole).toString();
    QString friend_nickname = index.data(IFriendListModel::FriendRole::NicknameRole).toString();
    if(m_chats.contains(friend_id)){
        QMessageBox::information(this,"好友列表","好友窗口已经打开");
        return;
    }
    ChatWindow* chat = new ChatWindow(friend_id,friend_nickname,m_network_tcp);
    // 连接子窗口信号
    connect(chat,&ChatWindow::chatWindowClosed,this,&FriendListWindow::do_chatWindowClosed);
    connect(this,&FriendListWindow::friendListClosed,chat,&ChatWindow::do_friendListClosed);
    connect(chat,&ChatWindow::chatWindowSendDialog,this,&FriendListWindow::do_chatWindowSendDialog);

    m_chats.insert(friend_id,chat);
    // 对红点信息进行修改
    m_list_model->handleChatWindowTime(friend_id);
}

/*
 * 3. sort键
 */
void FriendListWindow::pbtn_sortClicked(){

    handleSort();
}

/*
 * 4. 添加好友按键
 */
void FriendListWindow::pbtn_addFriendClicked(){
    const QString& friend_id = line_search->text();
    if(isValid(friend_id)){
        if(friend_id.isEmpty()){
            QMessageBox::information(this,"好友添加","空内容，请重新输入");
            return;
        }else if(friend_id == User::getInstance()->getId()){

            QMessageBox::information(this,"好友添加","不可以添加自己");
            return;
        }else{
            // 发送添加好友的信息
            friendListWindowAdd();
        }
    }

}

/*
 * 5. 删除好友的按键
 */
void FriendListWindow::pbtn_deleteFriendClicked(){

    const QString& friend_id = line_search->text();
    if(isValid(friend_id)){
        if(friend_id.isEmpty()){
            QMessageBox::information(this,"好友删除","空内容，请重新输入");
            return;
        }else{

            // 发送删除好友的消息
            friendListWindowDelete();
        }
    }
}

/*
 * 6. 打开设置窗口
 * 改进：
 *      1. 可以打开任意多个窗口，可以自动解决
 */
void FriendListWindow::pbtn_settingClicked(){
    // 打开设置窗口
    SettingWindow* setting_window = new SettingWindow(m_network_tcp);
    connect(this,&FriendListWindow::friendListClosed,setting_window,&SettingWindow::do_friendListClosed);
    connect(setting_window,&SettingWindow::logoutUser,this,&FriendListWindow::do_logoutUser);
    setting_window->show();
}

/*
 * 7. 用户退出
 */
void FriendListWindow::do_logoutUser(){
    m_exit_user = 1;
    this->close();
    emit loginShow();
}

/*
 * 8. 关闭聊天窗口
 */
void FriendListWindow::do_chatWindowClosed(const QString &friend_id){
    m_chats.remove(friend_id);  // 此时必有改项，内存会自己解决
}

/*
 * 9. 发送信息时更改列表
 */
void FriendListWindow::do_chatWindowSendDialog(const QString &friend_id, const QString &time){
    m_list_model->handleChatWindowSendDialog(friend_id,time);
}

/*
 * 有效性检测
 */
bool FriendListWindow::isValid(const QString &search){
    // 1. 判断是否有空白字符
    if (search.trimmed().isEmpty()) {
        QMessageBox::information(this, "用户搜索","输入为空，请重新输入");
        return false;
    }

    // 2. 判断是否包含不允许的特殊字符
    QRegularExpression specialChars("[\\s'#]"); // 正则表达式匹配空格、单引号和井号
    if (specialChars.match(search).hasMatch()) {
        QMessageBox::information(this, "用户搜索", "输入中不能含有'、#、空");
        return false;
    }

    // 3. 判断长度是否合法
    // 假设合法长度范围是大于0小于等于12
    if (search.length() <= 0 || search.length() > 12) {
        QMessageBox::information(this, "用户搜索", "ID长度必须在6到12个字符之间");
        return false;
    }

    // 如果所有检查都通过，则返回 true
    return true;
}

/*
 * 错误处理函数
 */
void FriendListWindow::errorOccurred(const QString &error) const{
    qDebug() << error;
}
