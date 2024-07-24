#include "FriendListProxyModel.h"
#include <QDebug>

/* * * * * * 1. 对象初始化和销毁 * * * * * */

/*
 * 1. 构造函数
 */
FriendListProxyModel::FriendListProxyModel(QObject *parent)
    : IFriendListProxyModel(parent), m_sort_column(0), m_sort_order(Qt::DescendingOrder){
    // 设置初始时按照默认排序
}

/*
 * 2. 析构函数
 */
FriendListProxyModel::~FriendListProxyModel(){
    qDebug() << "~FriendListProxyModel() called";
}


/* * * * * * 2. 接口函数的实现 * * * * * */

/*
 * 1. 设置源
 * 改进：
 *      1. 要确保设置原模型时数据源已经更新完毕——false
 */
void FriendListProxyModel::setSourceModel(QAbstractItemModel *source_model){
    QSortFilterProxyModel::setSourceModel(source_model);
}


/*
 * 2. 排序函数
 */
void FriendListProxyModel::sort(int column, Qt::SortOrder order){
    if(!sourceModel()){
        return;
    }else{
        m_sort_column = column;
        m_sort_order = order;
        invalidate();
        QSortFilterProxyModel::sort(0,order);
    }
}




/* * * * * * 3. 私有辅助函数 * * * * * */

/*
 * 1. lessThan
 */
bool FriendListProxyModel::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const{

    // 1. 获取源模型
    IFriendListModel* friend_model = qobject_cast<IFriendListModel*>(sourceModel());
    if(!friend_model){
        // NOTICE 一般不会出现这种情况
        return false;
    }

    // 2. 获取数据

    if (!source_left.isValid() || !source_right.isValid()) {
        errorOccurred("Invalid source index");
        return false;
    }


    QVariant left_data,right_data;
    // int sort_column = this->sortColumn();
    Qt::SortOrder sort_order = m_sort_order;
    switch(m_sort_column){
        // 3. 按照设置的column进行排序
    case 0:     // 3. 1 默认排序，先按照时间，再按照未读消息数量，降序排序
        // 3. 1. 1 按照时间进行排序
        left_data = friend_model->data(source_left,IFriendListModel::LastMsgTimeRole);
        right_data = friend_model->data(source_right,IFriendListModel::LastMsgTimeRole);
        if(left_data.toString() != right_data.toString()){
            return (sort_order == Qt::DescendingOrder) ?
                    QString::compare(left_data.toString(), right_data.toString()) < 0 :
                    QString::compare(left_data.toString(), right_data.toString()) > 0;
        }else{
            left_data = friend_model->data(source_left,IFriendListModel::UnReadMsgRole);
            right_data = friend_model->data(source_right,IFriendListModel::UnReadMsgRole);
            return (sort_order == Qt::DescendingOrder) ?
                       (left_data.toInt() < right_data.toInt()) :
                       (left_data.toInt() > right_data.toInt());
            // 等于不进行排序
        }

    case 1:
        // 3. 2 按照Id排序，升序排序，Id不会相同
        left_data = friend_model->data(source_left,IFriendListModel::IdRole);
        right_data = friend_model->data(source_right,IFriendListModel::IdRole);

        return (sort_order == Qt::AscendingOrder) ?
               QString::compare(left_data.toString(), right_data.toString()) < 0 :
               QString::compare(left_data.toString(), right_data.toString()) > 0;

    case 2:
        // 3. 3 按照nickname进行排序，升序排序
        left_data = friend_model->data(source_left,IFriendListModel::NicknameRole);
        right_data = friend_model->data(source_right,IFriendListModel::NicknameRole);
        if(left_data.toString() != right_data.toString()){
            return (sort_order == Qt::AscendingOrder) ?
                       QString::compare(left_data.toString(), right_data.toString()) < 0 :
                       QString::compare(left_data.toString(), right_data.toString()) > 0;
        }else{
            // 如果昵称相同，按照id尽心排序
            left_data = friend_model->data(source_left,IFriendListModel::IdRole);
            right_data = friend_model -> data(source_right,IFriendListModel::IdRole);
            return (sort_order == Qt::AscendingOrder) ?
                       QString::compare(left_data.toString(), right_data.toString()) < 0 :
                       QString::compare(left_data.toString(), right_data.toString()) > 0;
        }


    case 3:
        // 3. 4 按照消息数量进行排序，降序排序
        left_data = friend_model -> data(source_left,IFriendListModel::UnReadMsgRole);
        right_data = friend_model -> data(source_right,IFriendListModel::UnReadMsgRole);

        if(left_data.toInt() != right_data.toInt()){
            return (sort_order == Qt::DescendingOrder) ?
                       (left_data.toInt() < right_data.toInt()) :
                       (left_data.toInt() > right_data.toInt());
        }else{
            // 未读消息一样多，按照时间前后排序
            left_data = friend_model->data(source_left,IFriendListModel::LastMsgTimeRole);
            right_data = friend_model->data(source_right,IFriendListModel::LastMsgTimeRole);

            return (sort_order == Qt::DescendingOrder) ?
                       QString::compare(left_data.toString(), right_data.toString()) < 0 :
                       QString::compare(left_data.toString(), right_data.toString()) > 0;
        }

    default:
        // 4. 一般不会发生这种情况，return了也是错误的
        errorOccurred("cannot lessThan, unknown column");
        return false;
    }

}

/*
 * 错误处理函数
 */
void FriendListProxyModel::errorOccurred(const QString& error) const{
    qDebug() << error;
}
