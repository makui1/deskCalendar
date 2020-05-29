#include "todolist.h"
#include "storage.h"
#include "todoitem.h"
#include "todoview.h"
#include "todoedit.h"
#include "monthviewitem.h"

#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidgetItem>
#include <QVector>
#include <QList>
#include <QHeaderView>
#include <QScrollBar>
#include <QKeyEvent>

#include <QDebug>

TodoList::TodoList(const QDate &date, QWidget *parent) : QDialog(parent),mainLayout(new QVBoxLayout),controllerLayout(new QHBoxLayout),
    table(new QTableWidget(0,2)),btnComplete(new QPushButton("完成任务")),btnAddTodo(new QPushButton("添加任务")),_date(date)
{
    resize(500,500);
    setMinimumSize(size());
    QString title = QString("%1年%2月%3日的任务").arg(date.year()).arg(date.month()).arg(date.day());
    setWindowTitle(title);
    initializeTable();
    loadItems();
    initializeBtns();
    mainLayout->addWidget(table);
    mainLayout->addLayout(controllerLayout);
    setLayout(mainLayout);
}

TodoList::TodoList(const QString &query, QWidget *parent) : QDialog(parent),mainLayout(new QVBoxLayout),controllerLayout(new QHBoxLayout),
    table(new QTableWidget(0,2)),btnComplete(new QPushButton("完成任务")),btnAddTodo(new QPushButton("添加任务")),searchWindow(true),_query(query)
{
    resize(500,500);
    setMinimumSize(size());
    QString title("搜索结果");
    setWindowTitle(title);
    initializeTable();
    loadItems(query);
    initializeBtns();
    mainLayout->addWidget(table);
    mainLayout->addLayout(controllerLayout);
    setLayout(mainLayout);
    btnAddTodo->setEnabled(false);
}

TodoList::~TodoList()
{
    delete mainLayout;
}

void TodoList::addTodo()
{
    TodoEdit editor(nullptr,_date);
    if(editor.exec() == QDialog::Accepted){
        table->clear();
        QStringList header;
        header << "任务" << "时间";
        table->setHorizontalHeaderLabels(header);
        loadItems();
        emit dynamic_cast<MonthViewItem*>(parent())->updateAll();
    }
}

void TodoList::completeTodo()
{
    QList<QTableWidgetItem *> items = table->selectedItems();
    if(items.count() <= 0) return;
    for(auto item:items)
    {
        Storage::start()->del(item->data(Qt::UserRole).toInt());
        if(!item->data(Qt::UserRole<<1).isNull()) Storage::start()->delFile(item->data(Qt::UserRole<<1).toInt());
        table->clear();
        QStringList header;
        header << "任务" << "时间";
        table->setHorizontalHeaderLabels(header);
        searchWindow ? loadItems(_query) : loadItems();
        emit dynamic_cast<MonthViewItem*>(parent())->updateAll();
    }
}

void TodoList::resizeEvent(QResizeEvent *)
{
    table->setColumnWidth(0,table->width()/3-5);
    table->setColumnWidth(1,table->width()*2/3-5);
}

void TodoList::keyPressEvent(QKeyEvent *event)
{
    if(event->modifiers() == Qt::SHIFT){
        switch(event->key()){
        case Qt::Key_C:{
            completeTodo();
            break;
        }
        }
    }
}

void TodoList::initializeTable()
{
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    QStringList header;
    header << "任务" << "时间";
    table->setHorizontalHeaderLabels(header);
    table->verticalHeader()->setVisible(false);

    table->horizontalScrollBar()->setStyleSheet("QScrollBar{background:transparent; height:10px;}"
      "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
      "QScrollBar::handle:hover{background:gray;}"
      "QScrollBar::sub-line{background:transparent;}"
      "QScrollBar::add-line{background:transparent;}");

    table->verticalScrollBar()->setStyleSheet("QScrollBar{background:transparent; width: 10px;}"
      "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
      "QScrollBar::handle:hover{background:gray;}"
      "QScrollBar::sub-line{background:transparent;}"
      "QScrollBar::add-line{background:transparent;}");

    table->setFocusPolicy(Qt::NoFocus);
}

void TodoList::initializeBtns()
{
    connect(btnAddTodo,&QPushButton::clicked,this,&TodoList::addTodo);
    connect(btnComplete,&QPushButton::clicked,this,&TodoList::completeTodo);
    controllerLayout->addWidget(btnAddTodo);
    controllerLayout->addWidget(btnComplete);
}

void TodoList::loadItems()
{
    int count = 0;
    QVector<TodoItem> items = Storage::start()->get(_date);
    table->setRowCount(items.count());
    TodoView *v;
    for(auto &item:items)
    {
        QString time = item.getTodoTime().toString() + " - " + item.getEndTime().toString();
        QTableWidgetItem *wi = new QTableWidgetItem(time);
        wi->setBackgroundColor(item.getColor());
        wi->setData(Qt::UserRole,item.getID());
        if(item.isFromFile()) wi->setData(Qt::UserRole<<1,Storage::start()->getFileID(item));
        v = new TodoView;
        v->setItem(item);
        v->setSelectionEnable(false);
        table->setCellWidget(count,0,v);
        table->setItem(count++,1,wi);
    }
}

void TodoList::loadItems(const QString &query)
{
    int count = 0;
    QVector<TodoItem> items = Storage::start()->get(query);
    table->setRowCount(items.count());
    TodoView *v;
    for(auto &item:items)
    {
        QString time = item.getTodoTime().toString() + " - " + item.getEndTime().toString();
        QTableWidgetItem *wi = new QTableWidgetItem(time);
        wi->setBackgroundColor(item.getColor());
        wi->setData(Qt::UserRole,item.getID());
        if(item.isFromFile()) wi->setData(Qt::UserRole<<1,Storage::start()->getFileID(item));
        v = new TodoView;
        v->setItem(item);
        v->setSelectionEnable(false);
        table->setCellWidget(count,0,v);
        table->setItem(count++,1,wi);
    }
}
