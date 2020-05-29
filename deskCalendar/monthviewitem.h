#ifndef MONTHVIEWITEM_H
#define MONTHVIEWITEM_H
#include "todoitem.h"

#include <QWidget>
#include <QLabel>
#include <QDate>
#include <QVBoxLayout>

class TodoView;

class MonthViewItem : public QWidget
{
    Q_OBJECT

private:
    bool _enable = true;
    int _year,_month,_day;
    QVBoxLayout *todoViewLayout;
    const int displayNum = 5;
    int count = 0;

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

public:
    explicit MonthViewItem(QWidget *parent = nullptr);
    void setDate(int year,int month,int day);
    void setEnable(bool enable);
    int getMonth() const;
    bool enable() const;
    int getDay() const;
    QVector<TodoView*> getViewItems();

public slots:
    void updateTodoList();
    void clearTodoList();
    void clearSelected();

signals:
    void updateAll();
};

#endif // MONTHVIEWITEM_H
