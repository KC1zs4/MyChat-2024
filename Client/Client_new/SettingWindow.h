#ifndef SETTINGWINDOW_H
#define SETTINGWINDOW_H

#include <QWidget>
#include "NetworkTcp.h"
#include <QMessageBox>

namespace Ui {
class SettingWindow;
}

class SettingWindow : public QWidget
{
    Q_OBJECT


private:
    Ui::SettingWindow *ui;
    INetworkTcp* m_network_tcp; // 无需再进行分装了

public:
    explicit SettingWindow(INetworkTcp* network_tcp, QWidget* parent = nullptr);
    ~SettingWindow();

private:
    void handleSettingSave(const QJsonObject& json);
    void handleSettingLogout(const QJsonObject& json);

    bool isValid(const QString& str);
    // 小小提醒
    void errorOccurred(const QString& error);

signals:
    void logoutUser();

public slots:
    void do_friendListClosed();

private slots:
    void pbtn_returnClicked();
    void pbtn_saveClicked();
    void pbtn_logoutClicked();
    void do_settingAffair(const QJsonObject& json);
};


#endif // SETTINGWINDOW_H
