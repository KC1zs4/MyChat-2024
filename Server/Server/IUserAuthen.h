#ifndef IUSERAUTHEN_H
#define IUSERAUTHEN_H

#include <QObject>
#include <QTcpSocket>
#include <Message.h>

class IUserAuthen: public QObject{

    Q_OBJECT

public:
    IUserAuthen(QObject* parent = nullptr): QObject(parent){}
    virtual ~IUserAuthen(){}

    virtual bool RegisterUser(const QString& client_id, const QString& client_password) = 0;
    virtual bool loginUser(const QString& client_id, const QString& client_password) = 0;
    virtual bool logoutUser(const QString& client_id) = 0;
};

#endif // IUSERAUTHEN_H
