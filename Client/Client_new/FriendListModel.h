#ifndef FRIENDLISTMODEL_H
#define FRIENDLISTMODEL_H

#include "IFriendListModel.h"

class FriendListModel: public IFriendListModel{

    Q_OBJECT

private:
    QList<Friend> m_friends;
    // 网络接口在window中即可

public:

    explicit FriendListModel(QObject* parent = nullptr);
    ~FriendListModel();

    // 接口函数
    // QAbstractListModel接口
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    // FriendListWindow中使用的接口
    bool handleFriendListLoad(const QJsonObject& json) override;
    bool handleFriendListAdd(const QJsonObject& json) override;
    bool handleFriendListDelete(const QJsonObject& json) override;
    bool handleFriendListUpdate(const QJsonObject& json) override;
    bool handleFriendListSetting(const QJsonObject& json) override;
    bool handleChatUpdate(const QJsonObject &json) override;
    bool isContainFriend(const QString &friend_id) const override;
    void handleChatWindowSendDialog(const QString& friend_id,const QString& time) override;
    void handleChatWindowTime(const QString &friend_id) override;

private:
    // bool deleteLocalDialog(const QString& friend_id);
    void errorOccurred(const QString& error) const;


};


#endif // FRIENDLISTMODEL_H
