#ifndef TODO_H
#define TODO_H
#include <QString>
#include <QColor>
#include <QTime>
#include "global.h"

class TodoItem
{
public:
    TodoItem(){};
    TodoItem(int ID,const QString &todoText,int year,int month,int day,const QTime &todoTime,const QColor &todoColor,int dayPeriod = 0);
    TodoItem(int ID,int year,int month,const QString &todoText,int dayOfWeek,const QTime &todoTime,const QColor &todoColor);
    TodoItem(const TodoItem &item);
    void setYear(int year) {_year = year;}
    void setMonth(int month) {_month = month;}
    void setDay(int day) {_day = day;}
    void setText(const QString &Text) {text = Text;}
    void setStartTime(const QTime &startTime) {_todoTime = startTime;}
    void setPeriod(int period) {_dayPeriod = period;}
    void setEndTime(const QTime &endTime) {_endTime = endTime;}
    void setReminder(int advancedTime) {advancedRemindMinute = advancedTime;}
    void setPriority(Cal::Priority priority) {_priority = priority;}
    void setWeek(int w) {_dayOfWeek = w;}
    void setColor(const QColor &color) {_todoColor = color;}

    int getYear() const {return _year;}
    int getMonth() const {return _month;}
    int getDay() const {return _day;}
    QTime getTodoTime() const {return _todoTime;}
    QTime getEndTime() const {return _endTime;}
    QColor getColor() const {return _todoColor;}
    int getWeek() const {return _dayOfWeek;}
    int getPeriod() const {return _dayPeriod;}
    int getReminder() const {return advancedRemindMinute;}
    int getID() const {return id;}
    QString getText() const {return text;}
    Cal::Priority getPriority() const {return _priority;}

    bool operator ==(const TodoItem &item);
    bool isEqual(TodoItem &item);
    bool isFromFile() const;
    void setFromFile(bool on);

private:
    int id;
    QString text;
    int _year=-1,_month=-1,_day=-1,_dayOfWeek =  0;//0 表示不设置按周重复
    QTime _todoTime = QTime::currentTime();
    QTime _endTime = _todoTime;
    int advancedRemindMinute = 5;
    int _dayPeriod = 0;//0 表示不设置周期
    QColor _todoColor = Qt::white;
    Cal::Priority _priority = Cal::Priority::noPriority;
    bool fromFile = false;
};

#endif // TODO_H
