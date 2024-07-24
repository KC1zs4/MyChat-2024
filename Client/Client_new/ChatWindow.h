#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QWidget>
#include <QListView>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>

#include "ChatDelegate.h"
#include "ChatModel.h"
#include "NetworkTcp.h"


class ChatWindow : public QWidget
{
    Q_OBJECT

private:
    QString m_friend_id;
    QString m_friend_nickname;
    QString m_friend_email;
    QString m_friend_signature;

    QTextEdit* text_input;
    QPushButton* pbtn_send;
    QLabel* label_identity;
    QLabel* label_email;
    QLabel* label_signature;

    IChatDelegate* m_chat_delegate;
    IChatModel* m_chat_model;
    QListView* m_chat_view;

    INetworkTcp* m_network_tcp;

public:
    explicit ChatWindow(const QString& friend_id,const QString& friend_nickname, INetworkTcp* network_tcp, QWidget* parent = nullptr);
    ~ChatWindow();

    bool chatLoad(const QJsonObject& json);
    bool chatForward(const QJsonObject& json);

private:
    void initialUi();
    void errorOccurred(const QString& error);

signals:
    void chatWindowClosed(const QString& friend_id);
    void chatWindowSendDialog(const QString& friend_id,const QString& time);

public slots:

    // 1. FriendListWindow对象信号连接的槽函数
    void do_friendListClosed();

private slots:

    // 1. 控件槽函数
    void pbtn_sendClicked();


};
#endif // CHATWINDOW_H
