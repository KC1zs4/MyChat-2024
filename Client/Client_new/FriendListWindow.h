#ifndef FRIENDLISTWINDOW_H
#define FRIENDLISTWINDOW_H

#include <QWidget>
#include <QListView>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <QListView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>

#include "NetworkTcp.h"
#include "FriendListDelegate.h"
#include "FriendListProxyModel.h"
#include "FriendListModel.h"
#include "Message.h"
#include "User.h"
#include "SettingWindow.h"
#include "ChatWindow.h"

class FriendListWindow : public QWidget
{
    Q_OBJECT

private:

    // 页面控件
    // 1. 搜索框
    QLineEdit* line_search;
    // 2. 按钮
    QPushButton* pbtn_add_friend;
    QPushButton* pbtn_delete_friend;
    QPushButton* pbtn_sort;
    QPushButton* pbtn_setting;
    // 3. 下拉框
    QComboBox* combox_search_type;
    QComboBox* combox_sort_type;

    INetworkTcp* m_network_tcp; // 用于接受信息
    IFriendListModel* m_list_model;
    IFriendListProxyModel* m_list_proxy_model;
    IFriendListDelegate* m_list_delegate;
    QListView* m_list_view;

    int m_exit_user;    // 用于判断是否关闭LoginWindow窗口
    QMap<QString,ChatWindow*> m_chats;




public:
    explicit FriendListWindow(INetworkTcp* network_tcp, QWidget *parent = nullptr);
    ~FriendListWindow();

private:

    bool friendListWindowLoad();    // NOTICE 初始化时发出获取好友列表的信息
    bool friendListWindowAdd();     // NOTICE 添加好友的信息
    bool friendListWindowDelete();  // NOTICE 删除好友的信息
    bool chatUpdate(const QJsonObject& json); // NOTICE 更新聊天提示信息

    bool handleSort();

    bool isValid(const QString& search);

    // 错误处理
    void errorOccurred(const QString& error) const;

signals:

    void loginShow();
    void loginClose();
    void friendListClosed();

private slots:

    // 1. 控件槽函数
    void do_itemClicked(const QModelIndex& index);
    void pbtn_sortClicked();
    void pbtn_addFriendClicked();
    void pbtn_deleteFriendClicked();
    void pbtn_settingClicked();

    // 2. INetworkTcp的信号
    void do_friendAffair(const QJsonObject& json);
    void do_chatAffair(const QJsonObject& json);

    // 3. SettingWindow的信号
    void do_logoutUser();

    // 4. ChatWindowModel的关闭信号
    void do_chatWindowClosed(const QString& friend_id);
    void do_chatWindowSendDialog(const QString& friend_id, const QString& time);
};

#endif // FRIENDLISTWINDOW_H
