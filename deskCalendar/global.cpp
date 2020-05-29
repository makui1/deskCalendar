#include "global.h"

Global::Global()
{

}

QString Global::getWeekString(int weekNum)
{
    QString weekString;
    switch(weekNum){
    case 0:{
        weekString = "周一";
        break;
    }
    case 1:{
        weekString = "周二";
        break;
    }
    case 2:{
        weekString = "周三";
        break;
    }
    case 3:{
        weekString = "周四";
        break;
    }
    case 4:{
        weekString = "周五";
        break;
    }
    case 5:{
        weekString = "周六";
        break;
    }
    case 6:{
        weekString = "周日";
        break;
    }
    }

    return weekString;
}

QString Global::getMonthString(int index)
{
    QString str;
    switch(index){
    case 1:{
        str = "一月";
        break;
    }
    case 2:{
        str = "二月";
        break;
    }
    case 3:{
        str = "三月";
        break;
    }
    case 4:{
        str = "四月";
        break;
    }
    case 5:{
        str = "五月";
        break;
    }
    case 6:{
        str = "六月";
        break;
    }
    case 7:{
        str = "七月";
        break;
    }
    case 8:{
        str = "八月";
        break;
    }
    case 9:{
        str = "九月";
        break;
    }
    case 10:{
        str = "十月";
        break;
    }
    case 11:{
        str = "十一月";
        break;
    }
    case 12:{
        str = "十二月";
        break;
    }
    }

    return str;
}

QString Global::getPriorityString(int p)
{
    if(p == 0)
        return QString("[!!!]");
    if(p == 1)
        return QString("[!!]");
    if(p == 2)
        return QString("[!]");
    return QString("");
}

int Global::getYearIndex(int year)
{
    if(year == -1) return 0;
    return year-1999;
}

int Global::getMonthIndex(int month)
{
    if(month == -1) return 0;
    return month;
}

int Global::getDayIndex(int day)
{
    if(day == -1) return 0;
    return day;
}

int Global::getReminderIndex(int minute)
{
    switch(minute){
    case 5:
        return 0;
    case 10:
        return 1;
    case 15:
        return 2;
    case 30:
        return 3;
    }
}

int Global::getPeriodIndex(int period)
{
    if(period) return period>6 ? period/7 : period;
    return 0;
}

int Global::getUnitIndex(int period)
{
    return period>6 ? 1 : 0;
}
