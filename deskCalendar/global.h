#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>

namespace Cal {
enum ViewState{
    monthView,weekView,null
};
enum Priority{veryImportant,important,littleImportant,noPriority};
}

class Global
{    
public:
    Global();
    static QString getWeekString(int weekNum);
    static QString getMonthString(int index);
    static QString getPriorityString(int p);
    static int getYearIndex(int year);
    static int getMonthIndex(int month);
    static int getDayIndex(int day);
    static int getReminderIndex(int minute);
    static int getPeriodIndex(int period);
    static int getUnitIndex(int period);
};

#endif // GLOBAL_H
