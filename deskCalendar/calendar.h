#ifndef CALENDAR_H
#define CALENDAR_H

#include <QWidget>

class Calendar : public QWidget
{
    Q_OBJECT

public:
    Calendar(QWidget *parent = 0);
    ~Calendar();
};

#endif // CALENDAR_H
