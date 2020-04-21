#ifndef TODO_H
#define TODO_H
#include <QColor>
#include <QString>
#include <QDate>
#include <QTime>

//规则有待实现


class TodoItem
{
public:
    enum Priority{veryImportant,important,littleImportant,notImportant};

    TodoItem():TodoItem(QDate::currentDate()){};
    TodoItem(const QString &todoText,const QDate &todoDate,const QTime &todoTime,const QColor &textColor = Qt::white);
    explicit TodoItem(const QDate &todoDate);

    void setDate(const QDate &newDate);
    void setTime(const QTime &newTime);
    void setRemind(int advancedMinute);
    void setPriority(Priority todoPriority);

private:
    int id = 0;
    QColor itemTextColor = Qt::white;
    QString text = "";
    int year = -1,month = -1,day = -1,dayOfWeek = -1,
    hour = 0,minute = 0,second = 0;
    int advancedRemindMinute = 5;
    enum Priority priority = Priority::notImportant;
};

#endif // TODO_H
