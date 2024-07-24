#include "User.h"
#include <QDebug>
\
/* * * * * * 1. 构造函数和析构函数 * * * * * */

std::unique_ptr<User> User::instance = nullptr;

User::User(){
    user_id = "";
    user_nickname = "";
    user_email = "";
    user_signature = "";
}

User::~User(){
    qDebug() << "~User called";
}



/* * * * * * 2. set 函数 * * * * * */

User* User::getInstance(){

    // 如果添加多线程要考虑线程安全，客户端的不需要

    if(instance == nullptr){
        instance.reset(new User());
    }
    return instance.get();
}

void User::setId(const QString &id){
    user_id = id;
}

void User::setEmail(const QString &email){
    user_email = email;
}

void User::setNickname(const QString &nickname){
    user_nickname = nickname;
}

void User::setSignature(const QString &signature){
    user_signature = signature;
}



/* * * * * * 3. get函数 * * * * * */

QString User::getId(){
    return user_id;
}

QString User::getEmail(){
    return user_email;
}

QString User::getNickname(){
    return user_nickname;
}

QString User::getSignature(){
    return user_signature;
}
