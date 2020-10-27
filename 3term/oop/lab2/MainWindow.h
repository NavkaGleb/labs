#pragma once

#include <QMainWindow>
#include <QLayoutItem>
#include <QMap>
#include <QVector>
#include <QStandardItemModel>
#include <QTreeWidget>
#include <QHash>

#include "Task.h"
#include "CalendarForm.h"
#include "TaskListModel.h"
#include "TaskListDelegate.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    // constructor / destructor
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void on_dateButton_clicked();
    void on_priority_activated(int index);
    void on_newTask_returnPressed();
    void on_tasksContainer_clicked(const QModelIndex& index);
    void on_taskTime_editingFinished();
    void on_taskDate_editingFinished();
    void on_taskPriority_activated(int index);
    void on_taskName_returnPressed();
    void on_taskDescription_textChanged();
    void on_newTask_textChanged(const QString& arg);
    void on_listsContainer_itemClicked(QTreeWidgetItem* item, int column);
    void on_addList_clicked();
    void on_listsContainer_itemChanged(QTreeWidgetItem* item, int column);

    void on_listsContainer_itemDoubleClicked(QTreeWidgetItem *item, int column);

private:
    // fields
    Ui::MainWindow* _ui;
    CalendarForm* _calendarForm;
    Ng::Task* _currentTask;
    QHash<QString, int> _lists;
    QMap<int, QVector<Ng::Task*>> _tasks;
    TaskListModel* _taskListModel;
    TaskListDelegate* _taskListDelegate;
    QString _currentList;
    int _index;

    // private methods
    void initListsContainer();
    void initTasksContainer();
    void addTask(Ng::Task* task);
    void clear();
    void taskFormSetEnabled(bool enabled);
    void updateTasks();

    void resizeEvent(QResizeEvent* event);
};
