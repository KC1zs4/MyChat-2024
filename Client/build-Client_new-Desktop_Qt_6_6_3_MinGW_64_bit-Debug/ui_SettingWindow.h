/********************************************************************************
** Form generated from reading UI file 'SettingWindow.ui'
**
** Created by: Qt User Interface Compiler version 6.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGWINDOW_H
#define UI_SETTINGWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SettingWindow
{
public:
    QLabel *label_title;
    QLabel *label_nickname;
    QLabel *label_email;
    QLabel *label_signature;
    QLineEdit *line_nickname;
    QLineEdit *line_email;
    QLineEdit *line_signature;
    QPushButton *pbtn_return;
    QPushButton *pbtn_save;
    QPushButton *pbtn_logout;

    void setupUi(QWidget *SettingWindow)
    {
        if (SettingWindow->objectName().isEmpty())
            SettingWindow->setObjectName("SettingWindow");
        SettingWindow->resize(600, 400);
        label_title = new QLabel(SettingWindow);
        label_title->setObjectName("label_title");
        label_title->setGeometry(QRect(250, 60, 100, 30));
        QFont font;
        font.setPointSize(15);
        label_title->setFont(font);
        label_title->setAlignment(Qt::AlignCenter);
        label_nickname = new QLabel(SettingWindow);
        label_nickname->setObjectName("label_nickname");
        label_nickname->setGeometry(QRect(110, 100, 80, 20));
        QFont font1;
        font1.setPointSize(11);
        label_nickname->setFont(font1);
        label_email = new QLabel(SettingWindow);
        label_email->setObjectName("label_email");
        label_email->setGeometry(QRect(110, 180, 80, 20));
        label_email->setFont(font1);
        label_signature = new QLabel(SettingWindow);
        label_signature->setObjectName("label_signature");
        label_signature->setGeometry(QRect(110, 250, 80, 20));
        label_signature->setFont(font1);
        line_nickname = new QLineEdit(SettingWindow);
        line_nickname->setObjectName("line_nickname");
        line_nickname->setGeometry(QRect(120, 140, 360, 30));
        line_email = new QLineEdit(SettingWindow);
        line_email->setObjectName("line_email");
        line_email->setGeometry(QRect(120, 210, 360, 30));
        line_signature = new QLineEdit(SettingWindow);
        line_signature->setObjectName("line_signature");
        line_signature->setGeometry(QRect(120, 280, 360, 30));
        pbtn_return = new QPushButton(SettingWindow);
        pbtn_return->setObjectName("pbtn_return");
        pbtn_return->setGeometry(QRect(120, 340, 80, 25));
        pbtn_return->setFont(font1);
        pbtn_save = new QPushButton(SettingWindow);
        pbtn_save->setObjectName("pbtn_save");
        pbtn_save->setGeometry(QRect(260, 340, 80, 25));
        pbtn_save->setFont(font1);
        pbtn_logout = new QPushButton(SettingWindow);
        pbtn_logout->setObjectName("pbtn_logout");
        pbtn_logout->setGeometry(QRect(400, 340, 80, 25));
        pbtn_logout->setFont(font1);

        retranslateUi(SettingWindow);

        QMetaObject::connectSlotsByName(SettingWindow);
    } // setupUi

    void retranslateUi(QWidget *SettingWindow)
    {
        SettingWindow->setWindowTitle(QCoreApplication::translate("SettingWindow", "Form", nullptr));
        label_title->setText(QCoreApplication::translate("SettingWindow", "\347\224\250\346\210\267\350\256\276\347\275\256", nullptr));
        label_nickname->setText(QCoreApplication::translate("SettingWindow", "\347\224\250\346\210\267\346\230\265\347\247\260", nullptr));
        label_email->setText(QCoreApplication::translate("SettingWindow", "\347\224\250\346\210\267\351\202\256\347\256\261", nullptr));
        label_signature->setText(QCoreApplication::translate("SettingWindow", "\347\224\250\346\210\267\347\255\276\345\220\215", nullptr));
        pbtn_return->setText(QCoreApplication::translate("SettingWindow", "\350\277\224\345\233\236", nullptr));
        pbtn_save->setText(QCoreApplication::translate("SettingWindow", "\344\277\235\345\255\230", nullptr));
        pbtn_logout->setText(QCoreApplication::translate("SettingWindow", "\351\200\200\345\207\272\347\231\273\345\275\225", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SettingWindow: public Ui_SettingWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGWINDOW_H
