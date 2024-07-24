#ifndef IFRIENDLISTMODEL_H
#define IFRIENDLISTMODEL_H

#include <QAbstractListModel>
#include <QAbstractItemModel>
#include "Friend.h"
#include "EnumJson.h"
#include <QJsonArray>
#include <QDir>
#include <QFile>
#include "User.h"

class IFriendListModel: public QAbstractListModel{

    Q_OBJECT

public:

    enum FriendRole{
        IdRole = Qt::UserRole + 1,
        NicknameRole,
        IsOnlineRole,
        LastMsgTimeRole,
        UnReadMsgRole
    };

    Q_ENUMs(FriendRole);

public:

    explicit IFriendListModel(QObject* parent = nullptr) : QAbstractListModel(parent){}
    virtual ~IFriendListModel() {}

    // QAbstractListModel接口
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const = 0;
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const = 0;
    virtual QHash<int, QByteArray> roleNames() const = 0;

    // FriendListWindow中使用的接口
    virtual bool handleFriendListLoad(const QJsonObject& json) = 0;
    virtual bool handleFriendListAdd(const QJsonObject& json) = 0;
    virtual bool handleFriendListDelete(const QJsonObject& json) = 0;
    virtual bool handleFriendListUpdate(const QJsonObject& json) = 0;
    virtual bool handleFriendListSetting(const QJsonObject& json) = 0;
    virtual bool handleChatUpdate(const QJsonObject& json) = 0;
    virtual void handleChatWindowSendDialog(const QString& friend_id,const QString& time) = 0;
    virtual void handleChatWindowTime(const QString& friend_id) = 0;
    virtual bool isContainFriend(const QString& friend_id) const = 0;
};

Q_DECLARE_METATYPE(IFriendListModel::FriendRole)

#endif // IFRIENDLISTMODEL_H
