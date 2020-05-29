#include "calendar.h"
#include "monthviewitem.h"
#include "todoitem.h"
#include "todoview.h"
#include "storage.h"
#include "todolist.h"
#include "settingdialog.h"

#include <QDebug>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QIcon>
#include <QBitmap>
#include <QSpacerItem>
#include <QColor>
#include <QMouseEvent>
#include <QSystemTrayIcon>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QTextBrowser>
#include <QDir>

using namespace Cal;

void Calendar::updateMonthView()
{
    //更新顶部的当前月份标识
    currentMonthLabel->setText(QString("%1年%2月").arg(currentMonthDate.year()).arg(currentMonthDate.month()));

    //获取当前月视图下第一个格子对应的日期
    QDate temp = currentMonthDate.addDays(1-currentMonthDate.dayOfWeek());
    for (int y = 0; y < 6; ++y)
    {
        for (int x = 0; x < 7; ++x)
        {
            auto i = static_cast<MonthViewItem *>(monthViewGrid->itemAtPosition(y, x)->widget());
            i->setDate(temp.year(),temp.month(),temp.day());
            i->setEnable(temp.month() == currentMonthDate.month());//决定日期所对应的绘制颜色
            i->clearTodoList();//避免重复绘制Todo
            i->updateTodoList();
            temp = temp.addDays(1);
        }
    }
}

void Calendar::closeTran()
{
    setWindowFlags(flagBak);
    setWindowOpacity(1);
    show();
}

void Calendar::displayResult()
{
    TodoList list(findInput->text(),this);
    list.exec();
}

void Calendar::setOpacity()
{
    auto action = static_cast<QAction *>(sender());
    setWindowOpacity(action->text().remove('%').toDouble()/100);
}

void Calendar::onAboutAction()
{
    QMessageBox::aboutQt(this,"关于");
}

void Calendar::onHelpAction()
{
    QFile file(QDir::currentPath()+"/help.data");
    file.open(QIODevice::ReadOnly);
    QString str = QString::fromUtf8(file.readAll());
    QTextBrowser *brower = new QTextBrowser;
    brower->setText(str);
    brower->show();
}

void Calendar::onSettingAction()
{
    SettingDialog dlg;
    dlg.exec();
}

QVector<TodoItem> Calendar::getSelectedItem()
{
    QVector<TodoItem> items;
    for(int y = 0;y < 6;++y){
        for(int x= 0;x < 7;++x){
            auto i = static_cast<MonthViewItem *>(monthViewGrid->itemAtPosition(y, x)->widget());
            if(i->enable()){
                auto viewItems = i->getViewItems();
                for(auto &viewItem:viewItems)
                    items.append(viewItem->getItem());
            }
        }
    }

    return items;
}

void Calendar::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        leftButtonPressing = true;
        mousePos = event->globalPos();
        origin = pos();
    }
    setFocus();
    QWidget::mousePressEvent(event);
}

void Calendar::mouseMoveEvent(QMouseEvent *event)
{
    if(leftButtonPressing)
        move(origin+event->globalPos()-mousePos);
    QWidget::mouseMoveEvent(event);
}

void Calendar::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
        leftButtonPressing = false;
    QWidget::mouseReleaseEvent(event);
}

void Calendar::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(event->modifiers() == Qt::Modifier::CTRL)
    {
        setWindowFlags(windowFlags() | Qt::WindowTransparentForInput | Qt::WindowStaysOnBottomHint);
        setWindowOpacity(0.6);
        show();
        trayIcon->showMessage("提醒","桌面日历已嵌入桌面，你可以通过托盘菜单来取消嵌入桌面",QIcon(":/icons/myicon.ico"));
    }
    setFocus();
    QWidget::mouseDoubleClickEvent(event);
}

void Calendar::keyPressEvent(QKeyEvent *event)
{
    if(event->modifiers() == Qt::CTRL && event->key() == Qt::Key_H) this->hide();
    else if(event->modifiers() == Qt::SHIFT && event->key() == Qt::Key_C){
        auto items = getSelectedItem();
        for(auto item:items){
            Storage::start()->del(item.getID());
            if(item.isFromFile()) Storage::start()->delFile(Storage::start()->getFileID(item));
            updateMonthView();
        }
    }
}

Calendar::Calendar(QWidget *parent)
    : QWidget(parent)
{
    QSize msize(calendarWidth,calendarHeight);
    setFixedSize(msize);
    setWindowFlag(Qt::FramelessWindowHint);
    flagBak = windowFlags();
    this->setObjectName("Calendar");//为了成功设置Qt样式表
    setStyleSheet("QWidget#Calendar {background:"+calBackgroundColorRgb+"}");

    mainLayout = new QVBoxLayout();
    controller = new QHBoxLayout();
    monthViewGrid = new QGridLayout;
    weekLabelLayout = new QHBoxLayout();

    mainLayout->setMargin(0);
    monthViewGrid->setSpacing(1);
    monthViewGrid->setMargin(1);

    currentMonthLabel = new QLabel(this);
    currentMonthLabel->setAlignment(Qt::AlignCenter | Qt::AlignHCenter);
    currentMonthLabel->setText(QString("%1年%2月").arg(QDate::currentDate().year()).arg(QDate::currentDate().month()));
    currentMonthLabel->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Maximum);

    mainLayout->addLayout(controller);
    mainLayout->addWidget(currentMonthLabel);
    setLayout(mainLayout);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/icons/myicon.ico"));
    QMenu *opacityMenu = new QMenu("窗口透明度");
    for (int i = 90; i >= 50; i -= 10){
        QAction *action = new QAction(QString::number(i)+"%");
        connect(action,&QAction::triggered,this,&Calendar::setOpacity);
        opacityMenu->addAction(action);
    }
    QAction *closeTransparent = new QAction("关闭嵌入桌面");
    connect(closeTransparent,&QAction::triggered,this,&Calendar::closeTran);
    QAction *exitApp = new QAction("退出");
    connect(exitApp,&QAction::triggered,qApp,&QApplication::exit);
    QMenu *menu = new QMenu;
    menu->addMenu(opacityMenu);
    menu->addAction(closeTransparent);
    menu->addAction(exitApp);
    trayIcon->setContextMenu(menu);
    connect(trayIcon,&QSystemTrayIcon::activated,this,&Calendar::show);

    trayIcon->show();
    trayIcon->showMessage("提醒","欢迎您使用桌面日历",QIcon(":/icons/myicon.ico"));

    initController();
    initMonthView();
    toMonthView();
}

void Calendar::initController()
{
    QHBoxLayout *childLayout;

    btnPrev = new QPushButton(this);
    QPixmap pix = QPixmap(":/icons/btnPrev.png");
    btnPrev->setIcon(QIcon(":/icons/btnPrev.png"));
    btnPrev->setMask(pix.mask());
    connect(btnPrev,&QPushButton::clicked,this,&Calendar::toPrevMonth);
    btnToday = new QPushButton("今天",this);
    connect(btnToday,&QPushButton::clicked,this,&Calendar::setToday);
    btnNext = new QPushButton(this);
    btnNext->setIcon(QIcon(":/icons/btnNext.png"));
    connect(btnNext,&QPushButton::clicked,this,&Calendar::toNextMonth);
    childLayout = new QHBoxLayout;
    childLayout->setMargin(0);
    childLayout->setSpacing(0);
    childLayout->addWidget(btnPrev);
    childLayout->addWidget(btnToday);
    childLayout->addWidget(btnNext);
    controller->addLayout(childLayout);

    QSpacerItem *spacer = new QSpacerItem(calendarWidth*2/3,1);
    controller->addItem(spacer);

    findLabel = new QLabel("搜索");
    findInput = new QLineEdit;
    connect(findInput,&QLineEdit::returnPressed,this,&Calendar::displayResult);
    childLayout = new QHBoxLayout;
    childLayout->setMargin(0);
    childLayout->setSpacing(1);
    childLayout->addWidget(findLabel);
    childLayout->addWidget(findInput);
    controller->addLayout(childLayout);

    btnFilter = new QPushButton(this);
    btnFilter->setIcon(QIcon(":/icons/btnFilter.png"));
    //connect(btnFilter,&QPushButton::)
    controller->addWidget(btnFilter);

    bar = new QMenuBar;
    bar->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
    menuMore = new QMenu("···");
    menuMore->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
    bar->addMenu(menuMore);
    setting = new QAction("设置");
    connect(setting,&QAction::triggered,this,&Calendar::onSettingAction);
    help = new QAction("帮助");
    connect(help,&QAction::triggered,this,&Calendar::onHelpAction);
    about = new QAction("关于");
    connect(about,&QAction::triggered,this,&Calendar::onAboutAction);
    exitApp = new QAction("退出");
    connect(exitApp,&QAction::triggered,qApp,&QApplication::exit);
    menuMore->addAction(setting);
    menuMore->addAction(help);
    menuMore->addAction(about);
    menuMore->addAction(exitApp);
    controller->addWidget(bar);
}

Calendar::~Calendar()
{
    Storage::end();
}

void Calendar::initMonthView()
{
    //weekbar load
    for (int i = 0;i <= 6;++i){
        QLabel *weekLabel;
        QString color = (i==6||i==5)?"red":"black";
        weekLabel = new QLabel("<font color=" + color + "><strong>" + Global::getWeekString(i) + "</strong></font>");
        weekLabel->setAlignment(Qt::AlignCenter | Qt::AlignHCenter);
        weekLabel->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Maximum);
        weekLabelLayout->addWidget(weekLabel);
    }

    currentMonthDate = QDate(QDate::currentDate().year(),QDate::currentDate().month(),1);
    QDate temp = currentMonthDate.addDays(1-currentMonthDate.dayOfWeek());

    //day item load
    for(int y = 0;y < 6;++y){
        for (int x = 0;x <= 6;++x){
            auto dayItem = new MonthViewItem(this);
            dayItem->setDate(temp.year(),temp.month(),temp.day());
            dayItem->setEnable(temp.month() == currentMonthDate.month());
            dayItem->updateTodoList();
            monthViewGrid->addWidget(dayItem,y,x);
            temp = temp.addDays(1);
        }
    }
}

void Calendar::toMonthView()
{
    for(int x = 0;x <= 6;++x){
        auto i = static_cast<QLabel *>(weekLabelLayout->itemAt(x)->widget());
        i->show();
    }
    mainLayout->addLayout(weekLabelLayout);
    mainLayout->addLayout(monthViewGrid);
    for (int y = 0; y < 6; ++y)
    {
        for (int x = 0; x < 7; ++x)
        {
            auto i = static_cast<MonthViewItem *>(monthViewGrid->itemAtPosition(y, x)->widget());
            i->show();
        }
    }
    currentMonthDate = QDate(QDate::currentDate().year(),QDate::currentDate().month(),1);
}

void Calendar::toPrevMonth()
{
    currentMonthDate = currentMonthDate.addMonths(-1);
    updateMonthView();
}

void Calendar::toNextMonth()
{
    currentMonthDate = currentMonthDate.addMonths(1);
    updateMonthView();
}

void Calendar::setToday()
{
    currentMonthDate = QDate(QDate::currentDate().year(),QDate::currentDate().month(),1);
    updateMonthView();
}
