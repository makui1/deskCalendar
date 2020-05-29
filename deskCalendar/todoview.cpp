#include "todoview.h"
#include "monthviewitem.h"
#include "todoedit.h"
#include "global.h"
#include "storage.h"

#include <QPainter>
#include <QFont>
#include <QApplication>
#include <QMouseEvent>
#include <QDebug>
#include <QDesktopServices>

void TodoView::paintEvent(QPaintEvent *event)
{
    QPainter p(this);

    QFont f = p.font();
    f.setBold(true);
    p.setFont(f);

    p.setRenderHint(QPainter::Antialiasing);
    p.setBrush(QBrush(_item.getColor()));
    p.setPen(Qt::transparent);
    QRect mrect = this->rect();
    mrect.setWidth(mrect.width() - 1);
    mrect.setHeight(mrect.height() - 1);
    p.drawRoundedRect(mrect, 10, 10);
    p.setPen(Qt::black);

    if(_selected){
        p.fillRect(rect(),QColor(0,0,0,100));
    }
    p.drawText(QRect(mrect.left()+3,mrect.top()+3,mrect.width(),mrect.height()),_item.getText()+"   "+Global::getPriorityString(_item.getPriority()));

    p.end();
}

void TodoView::mousePressEvent(QMouseEvent *event)
{
    if(selectionEnable && event->modifiers() == Qt::Modifier::CTRL){
        if(event->button() == Qt::LeftButton){
            _selected = !_selected;
            update();
            return;
        }
    }
    else if(_item.isFromFile() && QApplication::keyboardModifiers() == Qt::ShiftModifier){
        emit clear();
        if(event->button() == Qt::LeftButton){
            QString path = Storage::start()->getFile(_item);
            if(path.isEmpty()) qDebug() << "Faild to find file!";
            else QDesktopServices::openUrl(QUrl::fromLocalFile(path));
        }
    }
    else{
        emit clear();
        if(event->button() == Qt::LeftButton){
            TodoEdit editor(&_item);
            editor.exec();
        }
    }
    setFocus();
    QWidget::mousePressEvent(event);
}

TodoView::TodoView(QWidget *parent) : QWidget(parent)
{
    if(parent)
        connect(this,&TodoView::clear,static_cast<MonthViewItem *>(parent),&MonthViewItem::clearSelected);
}

void TodoView::setItem(const TodoItem &item)
{
    _item = item;
}

const TodoItem &TodoView::getItem() const
{
    return _item;
}

void TodoView::clearSelect()
{
    _selected = false;
    update();
}

void TodoView::setSelectionEnable(bool on)
{
    selectionEnable = on;
}

bool TodoView::isSelected() const
{
    return _selected;
}

