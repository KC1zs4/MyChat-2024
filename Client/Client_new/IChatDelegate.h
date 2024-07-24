#ifndef ICHATDELEGATE_H
#define ICHATDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QFontMetrics>
#include "ChatModel.h"

class IChatDelegate: public QStyledItemDelegate{

    Q_OBJECT

public:
    IChatDelegate(QObject* parent = nullptr): QStyledItemDelegate(parent){}
    virtual ~IChatDelegate(){}

    // 接口函数
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const = 0;
    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const = 0;

};

#endif // ICHATDELEGATE_H
