#include "todoitem.h"
#include <QDebug>

TodoItem::TodoItem(int ID, const QString &todoText, int year, int month, int day, const QTime &todoTime, const QColor &todoColor, int dayPeriod):
    id(ID),text(todoText),_year(year),_month(month),_day(day),_todoTime(todoTime),_todoColor(todoColor),_dayPeriod(dayPeriod),_dayOfWeek(0)
{
    _endTime = todoTime;
}

TodoItem::TodoItem(int ID,int year,int month, const QString &todoText, int dayOfWeek, const QTime &todoTime, const QColor &todoColor):
    id(ID),text(todoText),_dayOfWeek(dayOfWeek),_todoTime(todoTime),_todoColor(todoColor),_year(year),_month(month),_day(0)
{
    _endTime = todoTime;
}

TodoItem::TodoItem(const TodoItem &i):id(i.id),text(i.text),_year(i._year),_month(i._month),_day(i._day),_todoTime(i._todoTime),_dayOfWeek(i.getWeek()),
    _endTime(i._endTime),advancedRemindMinute(i.advancedRemindMinute),_dayPeriod(i._dayPeriod),_todoColor(i._todoColor),_priority(i._priority),
    fromFile(i.fromFile)
{}

bool TodoItem::operator ==(const TodoItem &item)
{
    return (item.id == id);
}

bool TodoItem::isEqual(TodoItem &item)
{
    if(_year == item._year && _month == item._month && _day == item._day && _dayOfWeek == item._dayOfWeek && advancedRemindMinute == item.advancedRemindMinute &&
            text == item.text && _todoTime == item._todoTime && _endTime == item._endTime && _dayPeriod == item._dayOfWeek &&
            _todoColor == item._todoColor && _priority == item._priority)
        return true;
    return false;
}

bool TodoItem::isFromFile() const
{
    return fromFile;
}

void TodoItem::setFromFile(bool on)
{
    fromFile = on;
}
