#include "monthviewitem.h"
#include "todoview.h"
#include "todoedit.h"
#include "calendar.h"
#include "storage.h"
#include "todolist.h"

#include <QPalette>
#include <QPaintEvent>
#include <QPainter>
#include <QFont>
#include <QMouseEvent>
#include <QDebug>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QFileInfo>

void MonthViewItem::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect drawRect(QPoint(rect().x()+15,rect().y()+15),QSize(20,20));
    int dayofWK;

    painter.fillRect(rect(),QColor(255,255,255));

    QFont f;
    f = painter.font();
    f.setBold(true);
    painter.setFont(f);
    if(QDate(_year,_month,_day) == QDate::currentDate()){
        painter.setBrush(Qt::black);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.drawEllipse(QRect(QPoint(rect().x()+9,rect().y()+13),QSize(20,20)));
        painter.setPen(Qt::white);
        painter.drawText(drawRect,QString("%1").arg(_day));
    }
    else{
        QPen pen;
        if (_enable){
            if((dayofWK = QDate(_year,_month,_day).dayOfWeek()) == 6 || dayofWK == 7)
                pen.setColor(Qt::red);
        }
        else pen.setColor(QColor(160, 160, 164));
        painter.setPen(pen);
        painter.drawText(drawRect,QString("%1").arg(_day));
    }

    painter.end();
    QWidget::paintEvent(event);
}

void MonthViewItem::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton){
        TodoEdit editor(nullptr,QDate(_year,_month,_day));
        if(editor.exec() == QDialog::Accepted) emit updateAll();
    }
    else if(event->button() == Qt::LeftButton){
        clearSelected();
        emit updateAll();
    }
    setFocus();
    QWidget::mousePressEvent(event);
}

void MonthViewItem::dragEnterEvent(QDragEnterEvent *event)
{
    qDebug() << "DragEnter";
    const QMimeData *mime = event->mimeData();
    if(mime->hasUrls() || mime->hasText())
        event->accept();
    else{
        qDebug() << "reject";
        event->ignore();
        return;
    }

    if(!mime->hasUrls()) return;
    const QList<QUrl> urls = mime->urls();
    for (const auto &url : urls)
    {
        if (!url.isLocalFile())
        {
            event->ignore();
            return;
        }
    }
}

void MonthViewItem::dropEvent(QDropEvent *event)
{
    const QMimeData *mime = event->mimeData();
    if(mime->hasUrls()){
        event->acceptProposedAction();

        const QList<QUrl> urls = mime->urls();
        for (const auto &url : urls)
        {
            if (!url.isLocalFile())
            {
                event->ignore();
                return;
            }
        }

        for (const auto &url : urls)
        {
            QFileInfo info(url.toLocalFile());
            QString name = info.baseName();
            QString path = info.filePath();
            TodoEdit editor(nullptr,QDate(_year,_month,_day));
            editor.setRecommandText(name);
            editor.setFilePath(path);
            if(editor.exec() == QDialog::Accepted) emit updateAll();
        }
    }
    else if(mime->hasText()){
        event->acceptProposedAction();
        TodoEdit editor(nullptr,QDate(_year,_month,_day));
        editor.setRecommandText(mime->text());
        if(editor.exec() == QDialog::Accepted) emit updateAll();
    }
    else{
        event->ignore();
        return;
    }
    qDebug() << QDate(_year,_month,_day) << "dropped";
}

void MonthViewItem::mouseDoubleClickEvent(QMouseEvent *event)
{
    TodoList *list = new TodoList(QDate(_year,_month,_day),this);
    list->exec();
    delete list;
}

MonthViewItem::MonthViewItem(QWidget *parent) : QWidget(parent)
{
    todoViewLayout = new QVBoxLayout;
    todoViewLayout->setSpacing(1);
    todoViewLayout->setContentsMargins(1,35,1,1);
    setLayout(todoViewLayout);
    setAcceptDrops(true);

    connect(this,&MonthViewItem::updateAll,static_cast<Calendar *>(parent),&Calendar::updateMonthView);
}

void MonthViewItem::setDate(int year,int month,int day)
{
    _year = year;
    _month = month;
    _day = day;
    update();
}

void MonthViewItem::setEnable(bool enable)
{
    _enable = enable;
}

int MonthViewItem::getMonth() const
{
    return _month;
}

bool MonthViewItem::enable() const
{
    return _enable;
}

int MonthViewItem::getDay() const
{
    return _day;
}

QVector<TodoView *> MonthViewItem::getViewItems()
{
    QVector<TodoView *> views;
    int temp = 0;
    while(temp < count){
        auto i = dynamic_cast<TodoView *>(todoViewLayout->itemAt(temp++)->widget());
        views.append(i);
    }

    return views;
}

void MonthViewItem::updateTodoList()
{
    if(count == displayNum) return;
    QVector<TodoItem> items;
    TodoView *v;
    items = Storage::start()->get(QDate(_year,_month,_day));

    int temp = 0;
    while(temp < count){
        auto i = static_cast<TodoView *>(todoViewLayout->itemAt(temp++)->widget());
        for(auto item:items){
            if(item == i->getItem())
                items.removeOne(item);
        }
    }

    for (auto i:items){
        if(i.getWeek() != 0 && i.getWeek() != QDate(_year,_month,_day).dayOfWeek()) continue;
        v = new TodoView(this);
        v->setItem(i);
        todoViewLayout->addWidget(v);
        count++;
        if(count == displayNum) return;
    }

    items = Storage::start()->getByPeriod(QDate(_year,_month,_day));
    for(auto i:items){
        v = new TodoView;
        v->setItem(i);
        todoViewLayout->addWidget(v);
        count++;
        if(count == displayNum) return;
    }
}

void MonthViewItem::clearTodoList()
{
    while (count-- > 0){
        auto i = static_cast<TodoView *>(todoViewLayout->itemAt(0)->widget());
        i->close();
        todoViewLayout->removeWidget(i);
        delete i;
    }
    count = 0;
}

void MonthViewItem::clearSelected()
{
    int temp = 0;
    while(temp < count){
        auto i = static_cast<TodoView *>(todoViewLayout->itemAt(temp++)->widget());
        i->clearSelect();
    }
}
