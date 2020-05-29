#include "sqlitestorage.h"
#include "todoitem.h"

#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QTime>
#include <QColor>
#include <QSqlDriver>

using namespace Cal;

//数据库初始化操作
const QString SQLiteStorage::sqlFileName = "dcal.db3";
const QString SQLiteStorage::sqlFileInit =
        "CREATE TABLE IF NOT EXISTS File "
        "( ID INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE, "
        "Year INTEGER NOT NULL, Month INTEGER NOT NULL, Day INTEGER NOT NULL, DayOfWeek INTEGER NOT NULL, "
        "Period INTEGER NOT NULL, Name TEXT NOT NULL, Path TEXT NOT NULL);";
const QString SQLiteStorage::sqlTodoInit =
        "CREATE TABLE IF NOT EXISTS TodoList"
        "( ID INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE, "
        "Year INTEGER NOT NULL, Month INTEGER NOT NULL, Day INTEGER NOT NULL, "
        "DayOfWeek INTEGER NOT NULL, dayPeriod INTEGER NOT NULL,"
        "Name TEXT NOT NULL,todoTime TEXT NOT NULL,endTime TEXT NOT NULL,remindAhead INTEGER NOT NULL,"
        "r INTEGER NOT NULL,g INTEGER NOT NULL,b INTEGER NOT NULL,Priority INTEGER NOT NULL, FromFile INTEGER NOT NULL);";
const QString SQLiteStorage::sqlTodoIndexInit =
        "CREATE INDEX IF NOT EXISTS date ON TodoList (Year ASC,Month ASC,Day ASC,DayOfWeek ASC);";

//数据库查询操作
const QString SQLiteStorage::sqlGetTodoByDate =
        "SELECT * FROM TodoList WHERE("
        "((Year = -1 OR Year = :year) AND (Month = -1 OR Month = :month) AND (Day = -1 OR Day = :day))"
        " OR ((Year = -1 OR Year = :year) AND (Month = -1 OR Month = :month) AND (DayOfWeek = :dow))"
        ") ORDER BY `ID` ASC;";
const QString SQLiteStorage::sqlGetTodoByQuery =
        "SELECT * FROM TodoList WHERE("
        "Name LIKE :query"
        ") ORDER BY ID ASC;";
const QString SQLiteStorage::sqlGetAllTodo =
        "SELECT * FROM TodoList ORDER BY ID ASC;";
const QString SQLiteStorage::sqlGetFile =
        "SELECT * FROM File WHERE("
        "(Year = -1 OR Year = :year) AND (Month = -1 OR Month = :month) AND (Day = -1 OR Day = :day) "
        "AND DayOfWeek = :dow AND Period = :dp AND Name = :name"
        ") ORDER BY ID ASC";

//数据库更改操作
const QString SQLiteStorage::sqlAddTodo =
        "INSERT INTO TodoList (Year,Month,Day, DayOfWeek,dayPeriod, Name,todoTime,endTime,remindAhead,r,g,b,Priority,FromFile)"
        "VALUES (:year,:month, :day, :dow, :dp, :text, :start, :end, :reminder, :r, :g, :b, :p, :fromFile);";
const QString SQLiteStorage::sqlUpdateTodo =
        "UPDATE TodoList SET "
        "Year = :year, Month = :month, Day = :day, DayOfWeek = :dow, dayPeriod = :dp,"
        " r = :r, g = :g, b = :b, Priority = :p, Name = :text, todoTime = :start, endTime = :end "
        "WHERE ID = :id;";
const QString SQLiteStorage::sqlDelTodoById =
        "DELETE FROM `TodoList` WHERE `ID` = :id;";
const QString SQLiteStorage::sqlAddFile =
        "INSERT INTO File (Year,Month,Day,DayOfWeek,Period,Name,Path) "
        "VALUES (:year,:month,:day,:dow,:dp,:name,:path);";
const QString SQLiteStorage::sqlDelFile =
        "DELETE FROM File WHERE ID = :id;";
const QString SQLiteStorage::sqlSetName =
        "UPDATE File Set Name = :name WHERE ID = :id;";


QVector<TodoItem> SQLiteStorage::todoItems(QSqlQuery &q)
{
    QVector<TodoItem> items;
    while(q.next()){
        int year,month,day,week,dp,id;
        bool fromFile;

        id = q.value("ID").toInt();
        QString name = q.value("Name").toString();
        year = q.value("Year").toInt();
        month = q.value("Month").toInt();
        day = q.value("Day").toInt();
        week = q.value("DayOfWeek").toInt();
        dp = q.value("dayPeriod").toInt();
        QStringList s = q.value("todoTime").toString().split(':');
        QStringList e = q.value("endTime").toString().split(':');
        QTime todoTime(s[0].toInt(),s[1].toInt());
        QTime endTime(e[0].toInt(),e[1].toInt());
        int r,g,b;
        r = q.value("r").toInt();
        g = q.value("g").toInt();
        b = q.value("b").toInt();
        fromFile = q.value("FromFile").toBool();

        TodoItem i(id,name,year,month,day,todoTime,QColor(r,g,b),dp);
        i.setEndTime(endTime);
        i.setReminder(q.value("remindAhead").toInt());
        i.setPriority(Priority(q.value("Priority").toInt()));
        i.setWeek(week);
        i.setFromFile(fromFile);

        items.append(i);
    }

    return items;
}

void SQLiteStorage::blindTodoItem(QSqlQuery &q, const TodoItem &i)
{
    q.bindValue(":year",i.getYear());
    q.bindValue(":month",i.getMonth());
    q.bindValue(":day",i.getDay());
    q.bindValue(":dow",i.getWeek());
    q.bindValue(":dp",i.getPeriod());
    q.bindValue(":text",i.getText());
    q.bindValue(":start",i.getTodoTime().toString().remove(5,3));
    q.bindValue(":end",i.getEndTime().toString().remove(5,3));
    q.bindValue(":reminder",i.getReminder());
    int r,g,b;
    i.getColor().getRgb(&r,&g,&b);
    q.bindValue(":r",r);
    q.bindValue(":g",g);
    q.bindValue(":b",b);
    q.bindValue(":p",int(i.getPriority()));
    q.bindValue(":fromFile",int(i.isFromFile()));
}

SQLiteStorage::SQLiteStorage():db(QSqlDatabase::addDatabase("QSQLITE"))
{
    db.setDatabaseName(sqlFileName);
    db.open();
    QSqlQuery q(db);

    //Initialize
    if(!q.exec(sqlFileInit))
        qDebug() << q.lastError().text();
    if(!q.exec(sqlTodoInit))
        qDebug() << q.lastError().text();
    if(!q.exec(sqlTodoIndexInit))
        qDebug() << q.lastError().text();
}

void SQLiteStorage::add(const TodoItem &item)
{
    QSqlQuery q(db);
    if(!q.prepare(sqlAddTodo)) qDebug() << q.lastError().text();
    blindTodoItem(q,item);
    if(!q.exec()) qDebug() << q.lastError().text();
}

QVector<TodoItem> SQLiteStorage::get(const QDate &date)
{
    QSqlQuery q(db);
    if (!q.prepare(sqlGetTodoByDate)) qDebug() << q.lastError().text();

    q.bindValue(":year",date.year());
    q.bindValue(":month",date.month());
    q.bindValue(":day",date.day());
    q.bindValue(":dow",date.dayOfWeek());

    if(!q.exec()) qDebug() << q.lastError().text();

    return todoItems(q);
}

QVector<TodoItem> SQLiteStorage::getByPeriod(const QDate &date)
{
    QSqlQuery q(db);
    if(!q.exec(sqlGetAllTodo)) qDebug() << q.lastError().text();

    QVector<TodoItem> items;
    int year,month,day,p;
    while(q.next()){
        //当前遍历到的Todo没有设置周期时直接跳过
        p = q.value("dayPeriod").toInt();
        if(!p) continue;

        //当前Todo设置了周次时直接跳过
        day = q.value("Day").toInt();
        if(!day) continue;

        //给定日期早于当前遍历的Todo的起始日期时直接跳过
        //等于当前日期时也跳过（避免重复绘制）
        year = q.value("Year").toInt();
        month = q.value("Month").toInt();
        if (date.daysTo(QDate(year,month,day)) >= 0) continue;

        //否则获取该Todo并暂存到vector中以返回
        if(date.daysTo(QDate(year,month,day)) % p == 0){
            int id = q.value("ID").toInt();
            QString name = q.value("Name").toString();
            QStringList s = q.value("todoTime").toString().split(':');
            QStringList e = q.value("endTime").toString().split(':');
            QTime todoTime(s[0].toInt(),s[1].toInt());
            QTime endTime(e[0].toInt(),e[1].toInt());
            int r,g,b;
            r = q.value("r").toInt();
            g = q.value("g").toInt();
            b = q.value("b").toInt();

            TodoItem i(id,name,year,month,day,todoTime,QColor(r,g,b),p);
            i.setEndTime(endTime);
            i.setReminder(q.value("remindAhead").toInt());
            i.setPriority(Priority(q.value("Priority").toInt()));
            i.setFromFile(q.value("FromFile").toBool());

            items.append(i);
        }
    }

    return items;
}

QVector<TodoItem> SQLiteStorage::get(const QString &query)
{
    QSqlQuery q(db);
    if(!q.prepare(sqlGetTodoByQuery)) qDebug() << q.lastError().text();

    q.bindValue(":query","%"+query+"%");
    if(!q.exec()) qDebug() << q.lastError().text();

    return todoItems(q);
}

void SQLiteStorage::set(int id, const TodoItem &item)
{
    QSqlQuery q(db);
    if(!q.prepare(sqlUpdateTodo)) qDebug() << q.lastError().text();
    blindTodoItem(q,item);
    q.bindValue(":id",id);
    if(!q.exec()) qDebug() << q.lastError().text();
}

void SQLiteStorage::del(int id)
{
    QSqlQuery q(db);
    if(!q.prepare(sqlDelTodoById)) qDebug() << q.lastError().text();
    q.bindValue(":id",id);
    if(!q.exec()) qDebug() << q.lastError().text();
}

void SQLiteStorage::addFile(const TodoItem &item, const QString &path)
{
    QSqlQuery q(db);
    if(!q.prepare(sqlAddFile)) qDebug() << q.lastError().text();

    q.bindValue(":year",item.getYear());
    q.bindValue(":month",item.getMonth());
    q.bindValue(":day",item.getDay());
    q.bindValue(":dow",item.getWeek());
    q.bindValue(":dp",item.getPeriod());
    q.bindValue(":name",item.getText());
    q.bindValue(":path",path);

    if(!q.exec()) qDebug() << q.lastError().text();
}

QString SQLiteStorage::getFile(const TodoItem &item)
{
    QSqlQuery q(db);
    if(!q.prepare(sqlGetFile)) qDebug() << q.lastError().text();

    q.bindValue(":year",item.getYear());
    q.bindValue(":month",item.getMonth());
    q.bindValue(":day",item.getDay());
    q.bindValue(":dow",item.getWeek());
    q.bindValue(":dp",item.getPeriod());
    q.bindValue(":name",item.getText());

    if(!q.exec()) qDebug() << q.lastError().text();
    if(q.next()) return q.value("Path").toString();
    return QString();
}

int SQLiteStorage::getFileID(const TodoItem &item)
{
    QSqlQuery q(db);
    if(!q.prepare(sqlGetFile)) qDebug() << q.lastError().text();

    q.bindValue(":year",item.getYear());
    q.bindValue(":month",item.getMonth());
    q.bindValue(":day",item.getDay());
    q.bindValue(":dow",item.getWeek());
    q.bindValue(":dp",item.getPeriod());
    q.bindValue(":name",item.getText());

    if(!q.exec()) qDebug() << q.lastError().text();
    if(q.next()) return q.value("ID").toInt();
    return -1;
}

void SQLiteStorage::delFile(int id)
{
    QSqlQuery q(db);
    if(!q.prepare(sqlDelFile)) qDebug() << q.lastError().text();

    q.bindValue(":id",id);

    if(!q.exec()) qDebug() << q.lastError().text();
}

void SQLiteStorage::setNameInFile(int id, const QString &name)
{
    QSqlQuery q(db);
    if(!q.prepare(sqlSetName)) qDebug() << q.lastError().text();
    q.bindValue(":name",name);
    q.bindValue(":id",id);
    if(!q.exec()) qDebug() << q.lastError().text();
}
