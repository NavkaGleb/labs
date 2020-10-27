#pragma once

#include <QMainWindow>
#include <QLayoutItem>
#include <QMap>
#include <QVector>

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
    void on_taskName_returnPressed();
    void on_taskDescription_textChanged();
    void on_newTask_textChanged(const QString& arg);

private:
    // fields
    Ui::MainWindow* _ui;
    CalendarForm* _calendarForm;
    Ng::Task* _currentTask;
    QMap<QString, QVector<Ng::Task*>> _lists;
    TaskListModel* _taskListModel;
    TaskListDelegate* _taskListDelegate;

    // private methods
    void initListsContainer();
    void initTasksContainer();
    void addTask(Ng::Task* task);
    void clear();
    void taskFormSetEnabled(bool enabled);

    void resizeEvent(QResizeEvent* event);
};
