#ifndef ICHATMODEL_H
#define ICHATMODEL_H

#include <QAbstractListModel>
#include <QJsonArray>
#include "Dialog.h"
#include "User.h"

class IChatModel: public QAbstractListModel{
    Q_OBJECT

public:

    enum ChatRole{
        TimeRole = Qt::UserRole + 1,
        SenderIdRole,
        ContentRole
    };

public:
    IChatModel(QObject* parent = nullptr):QAbstractListModel(parent){}
    virtual ~IChatModel(){}

    // 接口函数
    // 1. 模型接口
    virtual int rowCount(const QModelIndex &parent) const = 0;
    virtual QVariant data(const QModelIndex &index, int role) const = 0;
    virtual QHash<int, QByteArray> roleNames() const = 0;
    // 2. 外部接口函数
    virtual bool handleChatLoad(const QJsonObject& json) = 0;
    virtual bool addDialog(const Dialog& dialog) = 0;


};


#endif // ICHATMODEL_H
