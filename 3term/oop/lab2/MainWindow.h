#pragma once

#include <QMainWindow>
#include <QLayoutItem>
#include <QMap>
#include <QVector>

#include "Task.h"
#include "CalendarForm.h"

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
    void on_newTask_returnPressed();

private:
    // fields
    Ui::MainWindow* _ui;
    CalendarForm* _calendarForm;
    Ng::Task* _currentTask;
    QMap<QString, QVector<Ng::Task*>> _lists;

    // private methods
    void initListsContainer();
    void initTasksContainer();
};
