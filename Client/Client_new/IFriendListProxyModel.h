#ifndef IFRIENDLISTPROXYMODEL_H
#define IFRIENDLISTPROXYMODEL_H

#include <QSortFilterProxyModel>
#include "FriendListModel.h"

class IFriendListProxyModel: public QSortFilterProxyModel{

    Q_OBJECT

public:

    IFriendListProxyModel(QObject* parent = nullptr): QSortFilterProxyModel(parent){}
    virtual ~IFriendListProxyModel(){}

    // 1. 接口函数
    virtual void setSourceModel(QAbstractItemModel* source_model) = 0;
    virtual void sort(int column, Qt::SortOrder order) = 0;
};

#endif // IFRIENDLISTPROXYMODEL_H
