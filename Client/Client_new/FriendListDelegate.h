#ifndef FRIENDLISTDELEGATE_H
#define FRIENDLISTDELEGATE_H

#include "IFriendListDelegate.h"

class FriendListDelegate: public IFriendListDelegate{

    Q_OBJECT

public:
    explicit FriendListDelegate(QObject* parent = nullptr);
    ~FriendListDelegate();

    // 接口函数
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:

    // 错误处理
    void errorOccurred(const QString& error) const;
};

#endif // FRIENDLISTDELEGATE_H
