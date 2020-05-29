#ifndef TODOLIST_H
#define TODOLIST_H
#include <QDialog>
#include <QDate>

class QTableWidget;
class QPushButton;
class QVBoxLayout;
class QHBoxLayout;

class TodoList : public QDialog
{
    Q_OBJECT
public:
    TodoList(const QDate &date,QWidget *parent = nullptr);
    TodoList(const QString &query,QWidget *parent = nullptr);
    ~TodoList();

public slots:
    void addTodo();
    void completeTodo();

protected:
    void resizeEvent(QResizeEvent *) override;
    void keyPressEvent(QKeyEvent *) override;

private:
    void initializeTable();
    void initializeBtns();
    void loadItems();
    void loadItems(const QString &query);

    QVBoxLayout *mainLayout;
    QHBoxLayout *controllerLayout;

    QTableWidget *table;

    QPushButton *btnComplete;
    QPushButton *btnAddTodo;

    QDate _date;
    bool searchWindow = false;
    QString _query;
};

#endif // TODOLIST_H
