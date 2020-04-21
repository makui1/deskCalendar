#include "todoitem.h"

TodoItem::TodoItem(const QString &todoText, const QDate &todoDate, const QTime &todoTime, const QColor &textColor)
    :text(todoText),year(todoDate.year()),month(todoDate.month()),day(todoDate.day()),
      hour(todoTime.hour()),minute(todoTime.minute()),second(todoTime.second()),
      itemTextColor(textColor) {};

TodoItem::TodoItem(const QDate &todoDate)
    :year(todoDate.day()),month(todoDate.month()),day(todoDate.day())
{
    QTime current = QTime::currentTime();
    hour = current.hour();
    minute = current.minute();
    second = current.second();
}

void TodoItem::setDate(const QDate &newDate)
{
    year = newDate.year();
    month = newDate.month();
    day = newDate.day();
}

void TodoItem::setTime(const QTime &newTime)
{
    hour = newTime.hour();
    minute = newTime.minute();
    second = newTime.second();
}

void TodoItem::setRemind(int advancedMinute)
{
    advancedRemindMinute = advancedMinute;
}
