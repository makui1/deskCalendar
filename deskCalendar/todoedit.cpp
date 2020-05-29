#include "todoedit.h"
#include "monthviewitem.h"
#include "storage.h"

#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDebug>
#include <QDate>
#include <QColorDialog>
#include <QMessageBox>
using namespace Cal;

TodoEdit::TodoEdit(TodoItem *item, QDate boxDate, QWidget *parent):QDialog(parent),dateBak(boxDate),
    itemBak(item),dateController(new QHBoxLayout),timeController(new QHBoxLayout),textReminderController(new QHBoxLayout),
    colorController(new QHBoxLayout),resultController(new QHBoxLayout),mainLayout(new QVBoxLayout)
{
    setWindowTitle(itemBak ? "事件编辑窗口" : "事件添加窗口");
    setWindowIcon(QIcon(":/icons/myicon.ico"));

    mainLayout->addLayout(dateController);
    mainLayout->addLayout(timeController);
    mainLayout->addLayout(textReminderController);
    mainLayout->addLayout(colorController);
    mainLayout->addLayout(resultController);
    setLayout(mainLayout);

    initEditor();
    setAllIndex();
}

void TodoEdit::setRecommandText(const QString &text)
{
    textInput->setText(text);
}

void TodoEdit::setFilePath(const QString &filePath)
{
    path = filePath;
}

void TodoEdit::setYear(int index)
{
    _item.setYear(index ? index+1999 : -1);
    updateDayBox();
}

void TodoEdit::setMonth(int index)
{
    _item.setMonth(index ? index : -1);
    updateDayBox();
}

void TodoEdit::setDay(int index)
{
    _item.setDay(index ? index : -1);
}

void TodoEdit::onSetWeek(int index)
{
    _item.setWeek(index);
}

void TodoEdit::setDateEnable(int index)
{
    dayBox->setEnabled(index == 0);
    periodBox->setEnabled(index == 0);
    _item.setPeriod(index ? 0 : (periodBox->currentIndex() * unitBox->itemData(unitBox->currentIndex()).toInt()));
}

void TodoEdit::onSetPeriod(int index)
{
    _item.setPeriod(index * unitBox->itemData(unitBox->currentIndex()).toInt());
}

void TodoEdit::onSetUnit(int index)
{
    _item.setPeriod(periodBox->currentIndex() * unitBox->itemData(index).toInt());
}

void TodoEdit::setStartHour(int index)
{
    QTime t(index,_item.getTodoTime().minute());
    _item.setStartTime(t);
}

void TodoEdit::setStartMinute(int index)
{
    QTime t(_item.getTodoTime().hour(),index);
    _item.setStartTime(t);
}

void TodoEdit::setEndHour(int index)
{
    QTime t(index,_item.getEndTime().minute());
    _item.setEndTime(t);
}

void TodoEdit::setEndMinute(int index)
{
    QTime t(_item.getEndTime().hour(),index);
    _item.setEndTime(t);
}

void TodoEdit::setReminder(int index)
{
    _item.setReminder(reminderBox->itemData(index).toInt());
}

void TodoEdit::setColor()
{
    QColor c = QColorDialog::getColor(_item.getColor(),this,"任务颜色选择");
    _item.setColor(c);
    int r,g,b;
    c.getRgb(&r,&g,&b);
    btnColorChooser->setStyleSheet("background:rgb("+QString::number(r)+","+QString::number(g)+","+QString::number(b)+")");
}

void TodoEdit::onBtnOK()
{
    if(textInput->text().isEmpty()){
        QMessageBox::warning(this,"警告","请输入任务文本！");
        return;
    }
    _item.setText(textInput->text());
    if(itemBak){
        Storage::start()->set(itemBak->getID(),_item);
        if(_item.isFromFile()) Storage::start()->setNameInFile(Storage::start()->getFileID(*itemBak),_item.getText());
    }
    else
    {
        if(!path.isEmpty()){
            qDebug() << "This is a file";
            _item.setFromFile(true);
            Storage::start()->addFile(_item,path);
        }
        Storage::start()->add(_item);
    }
    accept();
}

void TodoEdit::onBtnComplete()
{
    Storage::start()->del(itemBak->getID());
    if(itemBak->isFromFile()) Storage::start()->delFile(Storage::start()->getFileID(*itemBak));
    done(1);
}

void TodoEdit::setPriority(int index)
{
    _item.setPriority(Priority(index));
}

void TodoEdit::initEditor()
{
    yearBox = new QComboBox;
    yearBox->addItem("任意年",-1);
    for (int i = 2000;i <= 2100;++i)
        yearBox->addItem(QString::number(i),i);
    connect(yearBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setYear(int)));
    dateController->addWidget(yearBox);

    monthBox = new QComboBox;
    monthBox->addItem("任意月",-1);
    for (int i = 1;i <= 12;++i)
        monthBox->addItem(Global::getMonthString(i),i);
    connect(monthBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setMonth(int)));
    dateController->addWidget(monthBox);

    dayBox = new QComboBox;
    dayBox->addItem("任意日",-1);
    for(int i = 1;i <= 31;++i)
        dayBox->addItem(QString::number(i)+"日",i);
    connect(dayBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setDay(int)));
    dateController->addWidget(dayBox);

    weekLabel = new QLabel("每");
    dateController->addWidget(weekLabel);
    weekBox = new QComboBox;
    weekBox->addItem("周次",0);
    for (int i = 1;i <= 7;++i)
        weekBox->addItem(Global::getWeekString(i-1),i);
    connect(weekBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onSetWeek(int)));
    connect(weekBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setDateEnable(int)));
    dateController->addWidget(weekBox);


    periodLabel = new QLabel("每");
    dateController->addWidget(periodLabel);
    periodBox = new QComboBox;
    periodBox->addItem("不设置",0);
    for(int i = 1;i <= 6;++i)
        periodBox->addItem(QString::number(i),i);
    connect(periodBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onSetPeriod(int)));
    dateController->addWidget(periodBox);

    unitBox = new QComboBox;
    unitBox->addItem("天",1);
    unitBox->addItem("周",7);
    connect(unitBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onSetUnit(int)));
    dateController->addWidget(unitBox);

    startLabel = new QLabel("开始时间: ");
    startHourBox = new QComboBox;
    for (int i = 0;i <= 23;++i)
        startHourBox->addItem(QString::number(i),i);
    connect(startHourBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setStartHour(int)));
    startDotLabel = new QLabel(" : ");
    startMinuteBox = new QComboBox;
    for (int i = 0;i <= 59;++i)
        startMinuteBox->addItem(i >= 10 ? QString::number(i) : QString::number(0)+QString::number(i),i);
    connect(startMinuteBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setStartMinute(int)));
    endLabel = new QLabel("结束时间: ");
    endHourBox = new QComboBox;
    for (int i = 0;i <= 23;++i)
        endHourBox->addItem(QString::number(i),i);
    connect(endHourBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setEndHour(int)));
    endDotLabel = new QLabel(" : ");
    endMinuteBox = new QComboBox;
    for (int i = 0;i <= 59;++i)
        endMinuteBox->addItem(i >= 10 ? QString::number(i) : QString::number(0)+QString::number(i),i);
    connect(endMinuteBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setEndMinute(int)));
    timeController->addWidget(startLabel);
    timeController->addWidget(startHourBox);
    timeController->addWidget(startDotLabel);
    timeController->addWidget(startMinuteBox);
    timeController->addWidget(endLabel);
    timeController->addWidget(endHourBox);
    timeController->addWidget(endDotLabel);
    timeController->addWidget(endMinuteBox);

    textLabel = new QLabel("任务： ");
    textInput = new QLineEdit;
    reminderLabel = new QLabel("提醒： 提前");
    reminderBox = new QComboBox;
    reminderBox->addItem("5分钟",5);
    reminderBox->addItem("10分钟",10);
    reminderBox->addItem("15分钟",15);
    reminderBox->addItem("30分钟",30);
    connect(reminderBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setReminder(int)));
    textReminderController->addWidget(textLabel);
    textReminderController->addWidget(textInput);
    textReminderController->addWidget(reminderLabel);
    textReminderController->addWidget(reminderBox);

    colorLabel = new QLabel("颜色 ： ");
    btnColorChooser = new QPushButton;
    connect(btnColorChooser,&QPushButton::clicked,this,&TodoEdit::setColor);
    colorController->addWidget(colorLabel);
    colorController->addWidget(btnColorChooser);

    priorityLabel = new QLabel("优先级： ");
    priorityBox = new QComboBox;
    priorityBox->addItem("高优先级");
    priorityBox->addItem("中优先级");
    priorityBox->addItem("低优先级");
    priorityBox->addItem("无优先级");
    connect(priorityBox,SIGNAL(currentIndexChanged(int)),this,SLOT(setPriority(int)));
    colorController->addWidget(priorityLabel);
    colorController->addWidget(priorityBox);

    if(itemBak){
        btnComplete = new QPushButton;
        btnComplete->setText("完成");
        connect(btnComplete,&QPushButton::clicked,this,&TodoEdit::onBtnComplete);
        resultController->addWidget(btnComplete);
    }
    btnOK = new QPushButton;
    btnOK->setText(itemBak ? "更新" : "添加");
    connect(btnOK,&QPushButton::clicked,this,&TodoEdit::onBtnOK);
    resultController->addWidget(btnOK);
}

void TodoEdit::setAllIndex()
{
    yearBox->setCurrentIndex(Global::getYearIndex(itemBak ? itemBak->getYear() : dateBak.year()));
    monthBox->setCurrentIndex(Global::getMonthIndex(itemBak ? itemBak->getMonth() : dateBak.month()));
    if(itemBak && !itemBak->getDay()) dayBox->setEnabled(false);
    else dayBox->setCurrentIndex(Global::getDayIndex(itemBak ? itemBak->getDay() : dateBak.day()));
    weekBox->setCurrentIndex(itemBak ? itemBak->getWeek() : 0);
    periodBox->setCurrentIndex(itemBak ? Global::getPeriodIndex(itemBak->getPeriod()) : 0);
    periodBox->setCurrentIndex(itemBak ? Global::getUnitIndex(itemBak->getPeriod()) : 0);

    int sh,sm,eh,em;
    if(itemBak){
        sh = itemBak->getTodoTime().hour();
        sm = itemBak->getTodoTime().minute();
        eh = itemBak->getEndTime().hour();
        em = itemBak->getEndTime().minute();
    }
    else{
        QTime curTime = QTime::currentTime();
        eh = sh = curTime.hour();
        em = sm = curTime.minute();
    }
    startHourBox->setCurrentIndex(sh);
    startMinuteBox->setCurrentIndex(sm);
    endHourBox->setCurrentIndex(eh);
    endMinuteBox->setCurrentIndex(em);

    if(itemBak) textInput->setText(itemBak->getText());
    reminderBox->setCurrentIndex(Global::getReminderIndex(itemBak ? itemBak->getReminder() : 5));

    int r,g,b;
    (itemBak ? itemBak->getColor() : Qt::white).getRgb(&r,&g,&b);
    _item.setColor(QColor::fromRgb(r,g,b));
    btnColorChooser->setStyleSheet("background:rgb("+QString::number(r)+","+QString::number(g)+","+QString::number(b)+")");
    priorityBox->setCurrentIndex(itemBak ? itemBak->getPriority() : 3);
}

void TodoEdit::updateDayBox()
{
    int y = _item.getYear(),m = _item.getMonth();
    if(y > 0 && m > 0){
        int days = QDate(y,m,1).daysInMonth();
        int dis = days-dayBox->count()+1;
        if (dis < 0){
            while(dis++ < 0) dayBox->removeItem(dayBox->count()-1);
        }
        else if(dis > 0){
            while(dis-- > 0) dayBox->addItem(QString::number(dayBox->count())+"日");
        }
    }
}
