#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include "UserAuthen.h"
#include "User.h"
#include "FriendListWindow.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class LoginWindow;
}
QT_END_NAMESPACE

class LoginWindow : public QWidget
{
    Q_OBJECT

private:
    Ui::LoginWindow *ui;

    IUserAuthen* m_user_authen;


public:
    explicit LoginWindow(QWidget* parent = nullptr);
    ~LoginWindow();

private:

    bool isValid(const QString& client_id, const QString& client_password);
    void errorOccurred(const QString& error);

private slots:

    // 空间槽函数
    void pbtn_loginFunc();
    void pbtn_registerFunc();

    // IUserAuthen对象信号连接槽函数
    void do_loginAffair(const QJsonObject& json, INetworkTcp* network_tcp);
    void do_registerAffair(const QJsonObject& json);

    // FriendListWindow对象连接槽函数
    void do_loginShow();
    void do_loginClose();


};
#endif // LOGINWINDOW_H
