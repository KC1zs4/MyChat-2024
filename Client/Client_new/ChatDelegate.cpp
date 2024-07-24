#include "ChatDelegate.h"

/* * * * * * * 1. 构造函数和析构函数 * * * * * */

/*
 * 1. 构造函数
 */
ChatDelegate::ChatDelegate(const QString& friend_id, const QString& friend_nickname, QObject* parent)
    : IChatDelegate(parent), m_friend_id(friend_id), m_friend_nickname(friend_nickname){}

/*
 * 2. 析构函数
 */
ChatDelegate::~ChatDelegate(){
    qDebug() << "~ChatDelegate called";
}


/* * * * * * 2. public接口函数 * * * * * */

/*
 * 1. paint函数
 * 改进
 *      1. 需要限制nickname的长度，因为这里没有检查message_comment是否需要换行，默认不需要——true，在设置时进行处理了
 */
void ChatDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const{

    /*
     * 1. 绘画逻辑，按照不同item是己方还是对方来进行绘制
     *      1. 如果是己方，将消息使用绿色背景，注意换行
     *      2. 如果是对方，将消息使用黄色背景，注意换行
     *      3. 总的view大小是固定的，头像大小是固定的，消息大小是不固定的，需要根据消息长度来计算
     */

    // 1. 获取消息
    const QString& dialog_sender_id = index.data(ChatModel::SenderIdRole).toString();
    const QString& dialog_time = index.data(ChatModel::TimeRole).toString();
    const QString& dialog_content = index.data(ChatModel::ContentRole).toString();

    // 2. 检查是否是自己
    bool is_self = dialog_sender_id == User::getInstance()->getId();
    QString dialog_nickname = is_self ? User::getInstance()->getNickname() : m_friend_nickname;

    // 3. 全局设置
    painter->setRenderHint(QPainter::Antialiasing); // 设置反锯齿

    // 4. 绘制泡泡
    painter->save();

    // 4. 1 设置画笔
    painter->setPen(Qt::NoPen); // 暂时设置没有画笔边框
    QColor background_color = is_self?Qt::green : Qt::lightGray;
    QBrush brush(background_color);
    QFont font("Times New Roman",10);
    painter->setFont(font);

    // 4. 2 绘制泡泡
    QFontMetrics fm(font);
    QRect content_rect = fm.boundingRect(0, 0, option.rect.width() - 80, 0, Qt::TextWordWrap | Qt::AlignLeft | Qt::TextExpandTabs, dialog_content);
    QSize content_size(content_rect.width()+20,content_rect.height()+20);   // 添加边框
    int x1 = is_self ? option.rect.right() - content_size.width() - 20 : option.rect.left() + 20;
    int y1 = option.rect.top() + 20;

    // 4. 3 确定泡泡左右位置
    QRect bubble_rect = QRect(QPoint(x1,y1),content_size);
    painter->fillRect(bubble_rect, brush);

    // 4. 4 写字
    painter->setPen(Qt::black); // 使用黑笔来写
    content_rect.moveTo(bubble_rect.x()+10,bubble_rect.y()+10);
    QTextOption msg_option;
    msg_option.setAlignment(Qt::AlignLeft); // 设置左对齐
    msg_option.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);  // 设置自动换行，在过长中截断
    painter->drawText(content_rect, dialog_content,msg_option);  // 设置边距位10

    painter->restore();

    // 5. 添加对方nickname和时间，先时间后nickname 在泡泡上
    painter->save();
    QPen pen(Qt::black);
    painter->setPen(pen);
    QFont font2("Times New Roman",10);
    QFontMetrics fm2(font2);
    if(dialog_nickname.isEmpty()){
        dialog_nickname = "NUSET";
    }
    QString dialog_comment = "\t" + dialog_nickname + "----" + dialog_time + "\t";
    QRect friend_rect = fm2.boundingRect(dialog_comment);
    int x2 = is_self ? bubble_rect.right() - friend_rect.width() : bubble_rect.left();  // 显示在靠近右边/左边
    int y2 = bubble_rect.top() - friend_rect.height();  // 显示在bubble_rect上面
    friend_rect.moveTo(x2,y2);
    QTextOption info_option;
    info_option.setAlignment(Qt::AlignCenter);
    info_option.setWrapMode(QTextOption::WordWrap);
    painter->drawText(friend_rect,dialog_comment,info_option);
    painter->restore();
}

/*
 * 2. sizeHint函数
 * 更改：需要根据文本的高度来更改显示框的高度
 */
QSize ChatDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const{

    // 1. 根据消息来确定高度
    const QString& dialog_content = index.data(ChatModel::ContentRole).toString();
    QFont font("Times New Roman",10);
    QFontMetrics fm(font);
    QRect content_rect = fm.boundingRect(0, 0, option.rect.width() - 80, 0, Qt::TextWordWrap | Qt::AlignLeft | Qt::TextExpandTabs, dialog_content); // 获取泡泡框的大小了
    return QSize(option.rect.width(),content_rect.height()+40); // 上下流出20

}
