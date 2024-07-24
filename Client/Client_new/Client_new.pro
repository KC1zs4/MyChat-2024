QT += core
QT += gui
QT += network
QT += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ChatDelegate.cpp \
    ChatModel.cpp \
    ChatWindow.cpp \
    EnumJson.cpp \
    FriendListDelegate.cpp \
    FriendListModel.cpp \
    FriendListProxyModel.cpp \
    FriendListWindow.cpp \
    NetworkTcp.cpp \
    SettingWindow.cpp \
    User.cpp \
    UserAuthen.cpp \
    main.cpp \
    LoginWindow.cpp

HEADERS += \
    ChatDelegate.h \
    ChatModel.h \
    ChatWindow.h \
    Dialog.h \
    EnumJson.h \
    Friend.h \
    FriendListDelegate.h \
    FriendListModel.h \
    FriendListProxyModel.h \
    FriendListWindow.h \
    IChatDelegate.h \
    IChatModel.h \
    IFriendListDelegate.h \
    IFriendListModel.h \
    IFriendListProxyModel.h \
    INetworkTcp.h \
    IUserAuthen.h \
    LoginWindow.h \
    Message.h \
    NetworkTcp.h \
    SettingWindow.h \
    User.h \
    UserAuthen.h

FORMS += \
    LoginWindow.ui \
    SettingWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
