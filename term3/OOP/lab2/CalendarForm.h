#pragma once

#include <QDialog>

#include "Task.h"

namespace Ui { class CalendarForm; }

class CalendarForm : public QDialog {
    Q_OBJECT
public:
    // constructor / destructor
    explicit CalendarForm(QWidget* parent = nullptr);
    ~CalendarForm();

    // modifiers
    void setTask(Ng::Task& task);

    // public methods
    void clear();

private slots:
    void on_buttonBox_accepted();

private:
    // fields
    Ui::CalendarForm* _ui;
    Ng::Task* _task;
};
