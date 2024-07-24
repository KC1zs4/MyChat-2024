#ifndef USER_H
#define USER_H

#include <QString>
#include <memory>

class User{

private:

    static std::unique_ptr<User> instance;  // 使用智能指针进行内存管理

    QString user_id;
    QString user_nickname;
    QString user_email;
    QString user_signature;

private:
    User();

public:

    ~User();

    static User* getInstance();

    void setId(const QString& id);
    void setNickname(const QString& nickname);
    void setEmail(const QString& email);
    void setSignature(const QString& signature);

    QString getId();
    QString getEmail();
    QString getNickname();
    QString getSignature();

    User(const User&) = delete;
    User& operator=(const User&) = delete;

};


#endif // USER_H
