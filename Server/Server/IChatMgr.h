#ifndef ICHATMGR_H
#define ICHATMGR_H

#include <QObject>
#include <QJsonArray>
#include "Message.h"

class IChatMgr: public QObject{

    Q_OBJECT

public:
    IChatMgr(QObject* parent = nullptr):QObject(parent){}
    virtual ~IChatMgr(){}

    // 公共接口
    virtual bool handleChatLoad(const QJsonObject& json) = 0;
    virtual bool handleChatForward(const QJsonObject& json) = 0;
    virtual bool handleChatClose(const QJsonObject& json) = 0;

};

#endif // ICHATMGR_H
