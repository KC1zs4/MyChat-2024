#include "DatabaseMySQL.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <qDebug>
#include <QUuid>

/* * * * * * 1. DatabaseMySQL的初始化与销毁 * * * * * */

/*
 * 1. 构造函数
 * 参数：const QString& host, const QString& id, const QString& password, const QString& dbname
 * 返回值：无
 */
DatabaseMySQL::DatabaseMySQL(const QString& host,quint16 port, const QString& id, const QString& password, const QString& dbname, QObject* parent)
    : IDatabaseMySQL(parent), m_connection(QUuid::createUuid().toString())
{

    m_db = new QSqlDatabase();
    *m_db = QSqlDatabase::addDatabase("QMYSQL",m_connection);
    // QSqlDatabase::addDatabase("QMYSQL",m_connection);
    m_db->setHostName(host);
    m_db->setPort(port);
    m_db->setDatabaseName(dbname);
    m_db->setUserName(id);
    m_db->setPassword(password);

    if(!m_db->open()){
        // 1. 错误情况处理
        // qDebug() << "Cannot DatabaseMySQL";
        errorOccurred("cannot DatabaseMySQL DatabaseMySQL" + (m_db->lastError()).text());
    }
}


/*
 * 2. 析构函数
 * 功能：释放资源m_db
 * 参数：无
 * 返回值：无
 * 假设：
 */
DatabaseMySQL::~DatabaseMySQL(){

    m_db->close();
    // 多线程的时候这里需要开多条线进行io
    if(m_db->isOpen()){
        emit errorOccurred("cannot ~DatabaseMySQL, after close is open");
        // NOTICE 一般不会出现此种情况
    }else{
        QString connectionName = m_db->connectionName();
        delete m_db;
        QSqlDatabase::removeDatabase(connectionName);
    }
    qDebug() << "~DatabaseMySQL called";
}








/* * * * * * 2. DatabaseMySQL的接口实现 * * * * * */

/*
 * 1. 插入记录
 * 功能：增
 * 参数：const QString& table_name, const QMap<QString,QVariant>& data
 * 返回值：bool
 * 使用：插入 table 并设置 data
 *      1. 传入表名QString，字段-值QMap即可
 */
bool DatabaseMySQL::insertRecord(const QString &table_name, const QMap<QString, QVariant> &data){
    QString columns, values;
    for(auto iter = data.constBegin();iter!=data.constEnd();iter++){
        columns += iter.key() + ", ";
        values += "'" + iter.value().toString().replace("'","''") + "', ";
    }

    columns.chop(2);
    values.chop(2);
    // 去除两个最后的", "

    QString query = "INSERT INTO " + table_name + " (" + columns + ") VALUES (" + values + ")";
    /*
     *  1. query类似是 INSERT INTO user_authen (id, password) VALUES ('admin', 'admin')
     *      1. 使用QSqlQuery可以不在最后添加分号
     *      2. 传入的字符串中的单引号需要转义''
     *      3. 使用时只需要传入表名QString，字段-值QMap即可
     */

    QSqlQuery sql{*m_db};
    if(!sql.exec(query)){
        // qDebug() << "cannot insertRecord";
        sql.clear();
        errorOccurred("cannot DatabaseMySQL insertRecord " + sql.lastError().text());
        return false;
    }else{
        sql.clear();
        return true;
    }
}

/*
 * 2. 更新记录
 * 功能：改
 * 参数：const QString& table_name,const QString& condition, const QMap<QString,QVariant>& data
 * 返回值：bool
 * 使用：修改 table 中 condition 的 data
 *      1. 传入表名QString，条件值QString，修改字段-修改值QMap
 *      2. 传入的condition需要是 id = 'admin' 这样的等式而不只是值，还要考虑联合主键的情况
 *          1. 在传入前要构造where面的条件语句condition
 */
bool DatabaseMySQL::updateRecord(const QString &table_name, const QString &condition, const QMap<QString, QVariant> &data){

    QString update;
    for(auto iter = data.constBegin();iter!=data.constEnd();iter++){
        update += iter.key() + " = '" + iter.value().toString().replace("'","''") + "', ";
    }
    update.chop(2);

    QString query = "UPDATE " + table_name + " SET " + update + " WHERE " + condition;
    /*
     * 1. query类似是 UPDATE user_authen SET password = 'admin' WHERE id = 'admin'
     *      1. 使用QSqlQuery可以不在最后添加分号
     *      2. 传入的字符串中的单引号需要转义''
     *      3. 传入表名QString，条件QString，修改字段-修改值QMap
     */

    QSqlQuery sql(*m_db);
    if(!sql.exec(query)){
        // qDebug() << "cannot updateRecord";
        sql.clear();
        errorOccurred("cannot DatabaseMySQL updateRecord " + sql.lastError().text());
        return false;
    }else{
        sql.clear();
        return true;
    }
}

/*
 * 3. 删除记录
 * 功能：删
 * 参数：const QString& table_name,const QString& condition, const QMap<QString,QVariant>& data
 * 返回值：bool
 * 使用：删除 table 中符合 condition 的记录
 *      1. 传入表名QString，条件QString
 *      2. 条件是id = 'admin'这样的等式
 */
bool DatabaseMySQL::deleteRecord(const QString &table_name, const QString &condition){

    QString query = "DELETE FROM " + table_name + " WHERE " + condition;
    // query类似是 DELETE FROM user_authen WHERE id = 'admin'

    QSqlQuery sql(*m_db);
    if(!sql.exec(query)){
        // qDebug() << "cannot deleteRecord";
        sql.clear();
        errorOccurred("cannot DatabaseMySQL deleteRecords " + sql.lastError().text());
        return false;
    }else{
        sql.clear();
        return true;
    }
}

/*
 * 4. 查询记录
 * 功能：查单条记录
 * 参数：const QString& table_name,const QString& condition
 * 返回值：QMap<QString, QVariant>
 * 假设：
 *      1. 查询总是成功的——false
 *      2. 获取信息的那段代码可能有问题
 * 使用：查找 table_name 中 condition 的 字段-值并返回QMap对应
 *      1. 传入表名QString，条件QString
 */
QMap<QString, QVariant> DatabaseMySQL::selectRecord(const QString &table_name, const QString &condition) const {

    QMap<QString, QVariant> res;
    QString query = "SELECT * FROM " + table_name + " WHERE " + condition;
    // query类似是 SELECT * FROM user_authen WHERE id = 'admin’

    QSqlQuery sql(*m_db);
    if(!sql.exec(query)){
        // qDebug() << "cannot selectRecord";
        sql.clear();
        errorOccurred("cannot DatabaseMySQL selectRecord " + sql.lastError().text());
        return res;
    }else{
        if(sql.next()){
            for(int i = 0;i < sql.record().count();i++){
                res.insert(sql.record().fieldName(i),sql.value(i));
            }
            // emit recordSelected(res);
        }
        sql.clear();
        return res;
    }
}

/* 5. 查询记录
 * 功能：查多条记录
 * 参数：const QString& table_name,const QString& condition
 * 返回值：QList<QMap<QString,QVariant>>
 * 假设：
 *      1. 查询总是成功的——false
 *      2. 获取信息的代码可能有问题——false
 * 使用：查找 table_name 中所有符合 condition 的 字段-值并返回QList<QMap>对应
 */
QList<QMap<QString, QVariant>> DatabaseMySQL::selectRecords(const QString &table_name, const QString &condition) const {
    QList<QMap<QString, QVariant>> res;
    QString query = "SELECT * FROM " + table_name + " WHERE " + condition;
    // query类似是 SELECT * FROM user_authen WHERE id = 'admin’

    QSqlQuery sql(*m_db);
    if(!sql.exec(query)){
        // qDebug() << "cannot selectRecords";
        sql.clear();
        errorOccurred("cannot DatabaseMySQL selectRecords " + sql.lastError().text());
        return res;
    }else{
        while(sql.next()){
            QMap<QString, QVariant> row_map;
            for(int i = 0;i < sql.record().count();i++){
                row_map.insert(sql.record().fieldName(i),sql.value(i));
            }
            res.append(row_map);
        }
        // emit recordsSelected(res);
        sql.clear();
        return res;
    }
}









/* * * * * * 3. 私有函数 * * * * * */

/*
 * 1. 错误处理函数
 */
void DatabaseMySQL::errorOccurred(const QString& error) const {
    // 实现错误处理逻辑
    qDebug() << error;
}
