#ifndef CALENDAR_H
#define CALENDAR_H
#include <QWidget>
#include <QDate>
#include <QApplication>
#include <QDesktopWidget>
#include "global.h"

class QHBoxLayout;
class QVBoxLayout;
class QGridLayout;
class QPushButton;
class QLabel;
class QLineEdit;
class QSystemTrayIcon;
class QMenu;
class QMenuBar;
class TodoItem;

class Calendar : public QWidget
{
    Q_OBJECT
private:
    //布局
    QHBoxLayout *controller;
    QVBoxLayout *mainLayout;
    QHBoxLayout *weekLabelLayout;
    QGridLayout *monthViewGrid;

    //控制栏控件
    QLabel *currentMonthLabel;
    QLabel *findLabel;
    QLineEdit *findInput;
    QPushButton *btnPrev;
    QPushButton *btnNext;
    QPushButton *btnToday;
    QPushButton *btnFilter;
    QPushButton *btnWeekView;
    QPushButton *btnMonthView;

    QMenu *menuMore;
    QAction *setting;
    QAction *help;
    QAction *about;
    QAction *exitApp;
    QMenuBar *bar;

    QSystemTrayIcon *trayIcon;

    //辅助的日历状态记录变量
    QDate mondayDate;
    QDate currentMonthDate;
    QString styleSheetBak;

    //窗口移动辅助变量
    bool leftButtonPressing = false;
    QPoint origin;
    QPoint mousePos;

    //窗口标志备份，用来关闭嵌入桌面
    Qt::WindowFlags flagBak;

    //获取当前月份处于被选中状态的TodoView
    QVector<TodoItem> getSelectedItem();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

public:
    const QString calBackgroundColorRgb = QString("rgb(85, 170, 0)");
    const double calendarWidth = QApplication::desktop()->width()*0.75;
    const double calendarHeight = QApplication::desktop()->height()*0.85;

    Calendar(QWidget *parent = 0);

    //加载控制栏
    void initController();

    //初始化月视图
    void initMonthView();

    ~Calendar();

public slots:
    //切换到月视图
    void toMonthView();

    //月视图前一月
    void toPrevMonth();

    //月视图后一月
    void toNextMonth();

    //月视图切换到今天所在月
    void setToday();

    //更新月视图画面
    void updateMonthView();

    //关闭嵌入桌面
    void closeTran();

    //展示搜索结果
    void displayResult();

    void setOpacity();
    void onAboutAction();
    void onHelpAction();
    void onSettingAction();
};

#endif // CALENDAR_H
