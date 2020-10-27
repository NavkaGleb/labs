#include "CalendarForm.h"
#include "ui_CalendarForm.h"

#include <iostream>

// constructor / destructor
CalendarForm::CalendarForm(QWidget* parent)
    : QDialog(parent), _ui(new Ui::CalendarForm) {
    this->_ui->setupUi(this);
}

CalendarForm::~CalendarForm() {
    delete this->_ui;
}

// modifiers
void CalendarForm::setTask(Ng::Task& task) {
    this->_task = &task;
}

// public methods
void CalendarForm::clear() {
    this->_ui->calendar->setSelectedDate(QDate::currentDate());
    this->_ui->timeInput->clear();
}

// private slots
void CalendarForm::on_buttonBox_accepted() {
    this->_task->setDate(this->_ui->calendar->selectedDate());
    this->_task->setTime(this->_ui->timeInput->time());
}
