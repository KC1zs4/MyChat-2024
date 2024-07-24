#ifndef FRIENDLISTPROXYMODEL_H
#define FRIENDLISTPROXYMODEL_H

#include "IFriendListProxyModel.h"

class FriendListProxyModel: public IFriendListProxyModel{

    Q_OBJECT

private:
    int m_sort_column;
    Qt::SortOrder m_sort_order;

public:
    explicit FriendListProxyModel(QObject* parent = nullptr);
    ~FriendListProxyModel();

    // 1. 接口函数
    void setSourceModel(QAbstractItemModel* source_model);
    void sort(int column, Qt::SortOrder order) override;

private:

    void errorOccurred(const QString& error) const;

    bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const override;
    // bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override; 可以拓展过滤
};

#endif // FRIENDLISTPROXYMODEL_H
