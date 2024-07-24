#include <QCoreApplication>

#include "Server.h"
#include "SingleSession.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    /* * * * * * 1. 数据库连接测试 * * * * * */
    // IDatabaseMySQL* test = new DatabaseMySQL("127.0.0.1",3306,"root","chenkl,.1","qq",&a);
    // QString condition = "client_id = 'admin'";
    // qDebug() << test->selectRecord("user_info",condition);

    /* * * * * * 2. 程序运行main函数 * * * * * */
    Server* server = new Server(&a);
    server->startServer();

    return a.exec();
}
