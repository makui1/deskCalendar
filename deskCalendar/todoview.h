#ifndef TODOVIEW_H
#define TODOVIEW_H
#include <todoitem.h>

#include <QWidget>

class TodoView : public QWidget
{
    Q_OBJECT
private:
    TodoItem _item;
    bool _selected = false;
    bool selectionEnable = true;

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

public:
    explicit TodoView(QWidget *parent = nullptr);
    void setItem(const TodoItem &item);
    const TodoItem& getItem() const;
    void clearSelect();
    void setSelectionEnable(bool on);
    bool isSelected() const;

signals:
    void clear();
};

#endif // TODOVIEW_H
