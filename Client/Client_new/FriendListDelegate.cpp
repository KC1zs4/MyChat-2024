#include "FriendListDelegate.h"
#include <QPainter>

/* * * * * * 1. 对象初始化和销毁 * * * * * */

/*
 * 1. 构造函数
 */
FriendListDelegate::FriendListDelegate(QObject *parent)
    : IFriendListDelegate(parent){}

/*
 * 2. 析构函数
 */
FriendListDelegate::~FriendListDelegate(){
    qDebug() << "~FriendListDelegate called";
}



/* * * * * * 2. public接口实现 * * * * * */

/*
 * 1. paint 绘画函数
 */
void FriendListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const{

    const QString& friend_nickname = index.data(FriendListModel::NicknameRole).toString();
    const QString& friend_id = index.data(FriendListModel::IdRole).toString();
    const QString& friend_last_msg_time = index.data(FriendListModel::LastMsgTimeRole).toString();
    int friend_unread_msg = index.data(FriendListModel::UnReadMsgRole).toInt();
    bool friend_is_online = index.data(FriendListModel::IsOnlineRole).toBool();

    painter->setRenderHint(QPainter::Antialiasing);
    painter->save();

    // 1. 对未上线的好友加上背景色
    QColor background_color;

    if(option.state & QStyle::State_MouseOver){
        background_color = Qt::lightGray;
    }else{
        background_color = friend_is_online?Qt::white:Qt::darkGray;
    }
    QBrush background_brush(background_color);
    painter->setBrush(background_brush);
    painter->fillRect(option.rect, background_brush);

    painter->restore();

    // 2. 绘制item好友项
    painter->save();

    QFont font1("Times New Roman");
    QFontMetrics fm1(font1);
    painter->setFont(font1);

    // 2. 1. 绘制矩形边界
    QRect nickname_rect = fm1.boundingRect(friend_nickname);
    QRect id_rect = fm1.boundingRect(friend_id);
    QRect last_msg_time_rect = fm1.boundingRect(friend_last_msg_time);

    nickname_rect.moveTo(option.rect.left()+option.rect.width()/16,option.rect.top()+option.rect.height()/16);
    id_rect.moveTo(option.rect.left()+option.rect.width()/16,option.rect.bottom()-id_rect.height());
    last_msg_time_rect.moveTo(option.rect.right()-last_msg_time_rect.width()-option.rect.width()/16,option.rect.bottom()-id_rect.height());

    // 2. 2. 绘制文本
    painter->drawText(nickname_rect, friend_nickname);
    painter->drawText(id_rect, friend_id);
    painter->drawText(last_msg_time_rect, friend_last_msg_time);

    painter->restore();

    // 3. 绘制消息红点
    if(friend_unread_msg>0){
        painter->save();
        painter->setBrush(Qt::red);
        painter -> setPen(Qt::NoPen);

        // 3. 1. 绘制红点
        int red_dot_radius = 10, interval = 5;
        QPoint red_dot(option.rect.right()-red_dot_radius-interval,option.rect.bottom()-red_dot_radius-interval);
        painter -> drawEllipse(red_dot,red_dot_radius,red_dot_radius);

        // 3. 2. 绘制未读信息条数
        QFont font2("Times New Roman");
        QFontMetrics fm2(font2);
        painter->setFont(font2);
        QRect unread_msg_rect = fm2.boundingRect(QString::number(friend_unread_msg));
        unread_msg_rect.moveCenter(red_dot);

        painter->setPen(Qt::black);
        painter->drawText(unread_msg_rect, Qt::AlignCenter, QString::number(friend_unread_msg));

        painter->restore();
    }

    // 4. 绘制边框区分
    painter->save();
    QPen pen;
    pen.setColor(friend_is_online?Qt::darkGray : Qt::lightGray);
    pen.setWidth(2);
    painter->setPen(pen);
    painter->drawRect(option.rect.adjusted(0,0,-2,-2));
    painter->restore();

}


/*
 * 2. sizeHint
 */
QSize FriendListDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const{
    Q_UNUSED(index);

    return QSize(option.rect.width(),80);
}

/*
 * 错误处理函数
 */
void FriendListDelegate::errorOccurred(const QString &error) const{
    qDebug() << error;
}
