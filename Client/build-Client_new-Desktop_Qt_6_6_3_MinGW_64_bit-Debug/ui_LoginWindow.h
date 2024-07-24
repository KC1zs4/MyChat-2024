/********************************************************************************
** Form generated from reading UI file 'LoginWindow.ui'
**
** Created by: Qt User Interface Compiler version 6.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINWINDOW_H
#define UI_LOGINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LoginWindow
{
public:
    QLabel *label_title;
    QLabel *label_id;
    QLabel *label_password;
    QLineEdit *line_id;
    QLineEdit *line_password;
    QPushButton *pbtn_register;
    QPushButton *pbtn_login;
    QSpinBox *spin_port;
    QLabel *label_port;
    QLineEdit *line_host;

    void setupUi(QWidget *LoginWindow)
    {
        if (LoginWindow->objectName().isEmpty())
            LoginWindow->setObjectName("LoginWindow");
        LoginWindow->resize(600, 400);
        label_title = new QLabel(LoginWindow);
        label_title->setObjectName("label_title");
        label_title->setGeometry(QRect(200, 80, 200, 30));
        QFont font;
        font.setPointSize(15);
        label_title->setFont(font);
        label_title->setAlignment(Qt::AlignCenter);
        label_id = new QLabel(LoginWindow);
        label_id->setObjectName("label_id");
        label_id->setGeometry(QRect(125, 140, 70, 30));
        QFont font1;
        font1.setPointSize(12);
        label_id->setFont(font1);
        label_password = new QLabel(LoginWindow);
        label_password->setObjectName("label_password");
        label_password->setGeometry(QRect(125, 220, 150, 30));
        label_password->setFont(font1);
        line_id = new QLineEdit(LoginWindow);
        line_id->setObjectName("line_id");
        line_id->setGeometry(QRect(125, 170, 350, 40));
        line_password = new QLineEdit(LoginWindow);
        line_password->setObjectName("line_password");
        line_password->setGeometry(QRect(125, 250, 350, 40));
        line_password->setEchoMode(QLineEdit::Password);
        pbtn_register = new QPushButton(LoginWindow);
        pbtn_register->setObjectName("pbtn_register");
        pbtn_register->setGeometry(QRect(125, 330, 100, 35));
        pbtn_login = new QPushButton(LoginWindow);
        pbtn_login->setObjectName("pbtn_login");
        pbtn_login->setGeometry(QRect(375, 330, 100, 35));
        spin_port = new QSpinBox(LoginWindow);
        spin_port->setObjectName("spin_port");
        spin_port->setGeometry(QRect(500, 360, 80, 25));
        spin_port->setMaximum(10000);
        label_port = new QLabel(LoginWindow);
        label_port->setObjectName("label_port");
        label_port->setGeometry(QRect(500, 340, 80, 20));
        line_host = new QLineEdit(LoginWindow);
        line_host->setObjectName("line_host");
        line_host->setGeometry(QRect(500, 310, 80, 25));

        retranslateUi(LoginWindow);

        QMetaObject::connectSlotsByName(LoginWindow);
    } // setupUi

    void retranslateUi(QWidget *LoginWindow)
    {
        LoginWindow->setWindowTitle(QCoreApplication::translate("LoginWindow", "LoginWindow", nullptr));
        label_title->setText(QCoreApplication::translate("LoginWindow", "\346\254\242\350\277\216\346\235\245\345\210\260\347\231\273\345\275\225\347\225\214\351\235\242!!!", nullptr));
        label_id->setText(QCoreApplication::translate("LoginWindow", "\347\224\250\346\210\267ID", nullptr));
        label_password->setText(QCoreApplication::translate("LoginWindow", "\347\224\250\346\210\267\345\257\206\347\240\201", nullptr));
        pbtn_register->setText(QCoreApplication::translate("LoginWindow", "\346\263\250\345\206\214", nullptr));
        pbtn_login->setText(QCoreApplication::translate("LoginWindow", "\347\231\273\345\275\225", nullptr));
        label_port->setText(QCoreApplication::translate("LoginWindow", "\347\253\257\345\217\243", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LoginWindow: public Ui_LoginWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINWINDOW_H
