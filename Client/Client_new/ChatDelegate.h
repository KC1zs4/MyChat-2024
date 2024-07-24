#ifndef CHATDELEGATE_H
#define CHATDELEGATE_H

#include "IChatDelegate.h"

class ChatDelegate: public IChatDelegate
{
    Q_OBJECT

private:
    QString m_friend_id;
    QString m_friend_nickname;

public:
    ChatDelegate(const QString& friend_id,const QString& friend_nickname, QObject* parent = nullptr);
    ~ChatDelegate() override;

    // 接口函数
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

};

#endif // CHATDELEGATE_H
