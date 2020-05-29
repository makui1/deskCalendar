#ifndef TODOEDIT_H
#define TODOEDIT_H
#include "todoitem.h"
#include "global.h"

#include <QDialog>
#include <QColor>
#include <QTime>

class QHBoxLayout;
class QVBoxLayout;
class QComboBox;
class QPushButton;
class QLabel;
class QLineEdit;

class TodoEdit : public QDialog
{
    Q_OBJECT

private:
    QHBoxLayout *dateController;
    QHBoxLayout *timeController;
    QHBoxLayout *textReminderController;
    QHBoxLayout *colorController;
    QHBoxLayout *resultController;
    QVBoxLayout *mainLayout;

    QPushButton *btnColorChooser;
    QPushButton *btnOK;
    QPushButton *btnComplete;

    QComboBox *yearBox;
    QComboBox *monthBox;
    QComboBox *dayBox;
    QComboBox *weekBox;
    QComboBox *periodBox;
    QComboBox *unitBox;
    QComboBox *startHourBox;
    QComboBox *startMinuteBox;
    QComboBox *endHourBox;
    QComboBox *endMinuteBox;
    QComboBox *reminderBox;
    QComboBox *priorityBox;

    QLabel *startLabel;
    QLabel *endLabel;
    QLabel *startDotLabel;
    QLabel *endDotLabel;
    QLabel *colorLabel;
    QLabel *textLabel;
    QLabel *reminderLabel;
    QLabel *weekLabel;
    QLabel *periodLabel;
    QLabel *priorityLabel;

    QLineEdit *textInput;

    TodoItem *itemBak;
    TodoItem _item;
    QDate dateBak;
    QString path = "";

    void initEditor();
    void setAllIndex();
    void updateDayBox();

public:
    TodoEdit(TodoItem *item = nullptr,QDate boxDate = QDate::currentDate(),QWidget *parent = nullptr);
    void setRecommandText(const QString &text);
    void setFilePath(const QString &filePath);

public slots:
    void setYear(int index);
    void setMonth(int index);
    void setDay(int index);
    void onSetWeek(int index);
    void setDateEnable(int index);
    void onSetPeriod(int index);
    void onSetUnit(int index);
    void setStartHour(int index);
    void setStartMinute(int index);
    void setEndHour(int index);
    void setEndMinute(int index);
    void setReminder(int index);
    void setColor();
    void onBtnOK();
    void onBtnComplete();
    void setPriority(int index);
};

#endif // TODOEDIT_H
