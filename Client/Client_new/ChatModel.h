#ifndef CHATMODEL_H
#define CHATMODEL_H

#include "IChatModel.h"
#include "NetworkTcp.h"
#include <QFile>
#include <QDir>

class ChatModel: public IChatModel{

    Q_OBJECT

private:
    QList<Dialog> m_dialogs;

    int m_new_dialog;  // 用于标记第一条未存储消息的位置
    QString m_friend_id;

public:
    explicit ChatModel(const QString& friend_id, QObject* parent = nullptr);
    ~ChatModel() override;

    // 接口函数
    // 1. 模型接口函数
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    // 2. 外部接口函数
    bool handleChatLoad(const QJsonObject &json) override;
    bool addDialog(const Dialog &dialog) override;



private:
    bool loadLocalDialog();
    bool saveLocalDialog();
};


#endif // CHATMODEL_H
