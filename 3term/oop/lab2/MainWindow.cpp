#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QMessageBox>
#include <QGridLayout>
#include <QPushButton>
#include <QCalendarWidget>
#include <QTableView>
#include <QObject>
#include <iostream>

void setLayoutVisible(QLayoutItem* item, bool visible) {
    if (auto widget = item->widget())
        return widget->setVisible(visible);

    if (auto layout = item->layout())
        for (int i = 0; i < layout->count(); ++i)
            setLayoutVisible(layout->itemAt(i), visible);
};

// constructor / destructor
MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), _ui(new Ui::MainWindow) {
    this->_ui->setupUi(this);
    this->_lists.append(ng::List("today"));
    this->_lists.append(ng::List("tomorrow"));

    setLayoutVisible(this->_ui->calendarForm, false);

    for (int i = 0; i < this->_lists.size(); ++i) {
        QPushButton* button = new QPushButton(this);
        button->setText(this->_lists[i].name());
        button->setFixedSize(100, 50);
        button->move(20, (i + 1) * 60);
        button->show();
    }
}

MainWindow::~MainWindow()
{
    delete this->_ui;
}

// private slots
void MainWindow::on_newTask_editingFinished() {
    this->_lists.front().append(ng::Task(this->_ui->newTask->text()));
    this->_ui->newTask->clear();

    this->_lists.front().print();
}

void MainWindow::on_taskDate_clicked() {
   setLayoutVisible(this->_ui->calendarForm, true);
   this->_ui->calendar->showToday();
}

void MainWindow::on_calendar_selectionChanged() {
    QDate date = this->_ui->calendar->selectedDate();

    std::cout << date.day() << " " << date.month() << " " << date.year() << std::endl;
}

void MainWindow::on_pushButton_clicked() {
    setLayoutVisible(this->_ui->calendarForm, false);
}
