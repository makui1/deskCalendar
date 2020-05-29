#ifndef SQLITESTORAGE_H
#define SQLITESTORAGE_H
#include <QString>
#include <QSqlDatabase>

class TodoItem;
class QDate;

class SQLiteStorage
{
private:
    //数据库连接
    QSqlDatabase db;

    //获取数据库query中的item
    QVector<TodoItem> todoItems(QSqlQuery &q);

    //填充query中的占位符
    void blindTodoItem(QSqlQuery &q,const TodoItem &item);

public:
    //数据库操作
    static const QString sqlFileName;
    static const QString sqlFileInit;
    static const QString sqlTodoInit;
    static const QString sqlTodoIndexInit;
    static const QString sqlAddTodo;
    static const QString sqlGetTodoByDate;
    static const QString sqlGetTodoByQuery;
    static const QString sqlGetAllTodo;
    static const QString sqlUpdateTodo;
    static const QString sqlDelTodoById;

    static const QString sqlAddFile;
    static const QString sqlGetFile;
    static const QString sqlDelFile;
    static const QString sqlSetName;

    SQLiteStorage();

    //将item存储到数据库中
    void add(const TodoItem &item);

    //获取给定日期或周次的Todo
    QVector<TodoItem> get(const QDate &date);

    //获取周期内包含给定日期的Todo
    QVector<TodoItem> getByPeriod(const QDate &date);

    //获取任务文本中包含query的Todo
    QVector<TodoItem> get(const QString &query);

    //更新给定id的Todo
    void set(int id,const TodoItem &item);

    //删除给定id的Todo
    void del(int id);

    void addFile(const TodoItem &item,const QString &path);
    QString getFile(const TodoItem &item);
    int getFileID(const TodoItem &item);
    void delFile(int id);
    void setNameInFile(int id,const QString &name);

};

#endif // SQLITESTORAGE_H
