#ifndef IFRIENDLISTDELEGATE_H
#define IFRIENDLISTDELEGATE_H

#include <QStyledItemDelegate>
#include "FriendListModel.h"

class IFriendListDelegate: public QStyledItemDelegate{

    Q_OBJECT

public:
    IFriendListDelegate(QObject* parent = nullptr) : QStyledItemDelegate(parent){}
    virtual ~IFriendListDelegate() {}

    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const = 0;
    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const = 0;
};

#endif // IFRIENDLISTDELEGATE_H
