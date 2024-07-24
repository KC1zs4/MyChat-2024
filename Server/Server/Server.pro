QT += core
QT += network
QT += sql

CONFIG += c++17 cmdline

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        ChatMgr.cpp \
        DatabaseMySQL.cpp \
        EnumDatabase.cpp \
        EnumJson.cpp \
        FriendMgr.cpp \
        NetworkTcp.cpp \
        Server.cpp \
        SingleSession.cpp \
        UserAuthen.cpp \
        main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    ChatMgr.h \
    DatabaseMySQL.h \
    EnumDatabase.h \
    EnumJson.h \
    FriendMgr.h \
    IChatMgr.h \
    IDatabaseMySQL.h \
    IFriendMgr.h \
    INetworkTcp.h \
    IUserAuthen.h \
    Message.h \
    NetworkTcp.h \
    Server.h \
    SingleSession.h \
    UserAuthen.h
