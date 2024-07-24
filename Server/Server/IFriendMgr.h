#ifndef IFRIENDMGR_H
#define IFRIENDMGR_H

#include <QObject>
#include "Message.h"
#include "EnumJson.h"

class IFriendMgr: public QObject{

    Q_OBJECT

public:
    IFriendMgr(QObject* parent = nullptr): QObject(parent){}
    virtual ~IFriendMgr(){}

    // 接口
    virtual bool handleFriendListLoad(const QJsonObject& json) = 0;
    virtual bool handleFriendListAdd(const QJsonObject& json) = 0;
    virtual bool handleFriendListDelete(const QJsonObject& json) = 0;
    virtual bool handleFriendListUpdate(const QJsonObject& json) = 0;
    virtual bool handleFriendListSetting(const QJsonObject& json) = 0;

    virtual bool handleNewFriendComing(const QJsonObject& json) = 0;
    virtual bool handleDelFriendComing(const QJsonObject& json) = 0;

signals:

    // 可以合成一个信号的，在singleSession调用不同子方法handle时区分即可
    void newFriendMgr(const QJsonObject& json, const QString& friend_id);
    void delFriendMgr(const QJsonObject& json, const QString& friend_id);

};

#endif // IFRIENDMGR_H
