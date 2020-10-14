#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QMessageBox>
#include <QGridLayout>
#include <QPushButton>
#include <QCalendarWidget>
#include <QTableView>
#include <QLineEdit>
#include <QObject>
#include <QStringList>
#include <QListWidgetItem>
#include <iostream>

// constructor / destructor
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), _ui(new Ui::MainWindow), _currentTask(new Ng::Task) {
    this->_ui->setupUi(this);
    this->setWindowTitle("to do list");
    this->_calendarForm = new CalendarForm(this);
    this->_calendarForm->setModal(true);

    this->_lists["today"];
    this->_lists["tomorrow"];

    this->_lists["today"].append(new Ng::Task("first"));
    this->_lists["today"].append(new Ng::Task("second"));

    this->initListsContainer();
    this->initTasksContainer();
}

MainWindow::~MainWindow() {
    delete this->_ui;

    for (auto& [listName, tasks] : this->_lists.toStdMap())
        for (auto& task : tasks)
            delete task;
}

// private slots
void MainWindow::on_dateButton_clicked() {
    if (!this->_currentTask)
        this->_currentTask = new Ng::Task;

    this->_calendarForm->setTask(*this->_currentTask);
    this->_calendarForm->show();
    this->_calendarForm->exec();
}

void MainWindow::on_newTask_returnPressed() {
    if (this->_ui->newTask->text().isEmpty())
        return;

    std::cout << "fuck this shit" << std::endl;
    std::cout << "current task = " << this->_currentTask << std::endl;

    if (!this->_currentTask)
        this->_currentTask = new Ng::Task;

    this->_currentTask->name(this->_ui->newTask->text());
    this->_lists["today"].append(this->_currentTask);

    QString value = this->_currentTask->name();
    QListWidgetItem* item = new QListWidgetItem;

    if (this->_currentTask->time())
        value += this->_currentTask->time()->toString();

    if (this->_currentTask->date())
        value += this->_currentTask->date()->toString();

    item->setText(value);
    item->setCheckState(Qt::Unchecked);
    this->_ui->tasksContainer->addItem(item);

    this->_ui->newTask->clear();
    this->_currentTask = nullptr;
}

// private methods
void MainWindow::initListsContainer() {
    this->_ui->listsContainer->setAlignment(Qt::AlignCenter | Qt::AlignTop);
    this->_ui->listsContainer->setGeometry(QRect(0, 100, 100, 800));

    for (const auto& list : this->_lists.keys()) {
        QPushButton* button = new QPushButton(this);

        button->setText(list);
        button->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        button->setMinimumHeight(50);
        this->_ui->listsContainer->addWidget(button);
    }
}

void MainWindow::initTasksContainer() {
    for (const auto& task : this->_lists["today"]) {
        QListWidgetItem* item = new QListWidgetItem;

        item->setText(task->name());
        item->setCheckState(Qt::Unchecked);
        this->_ui->tasksContainer->addItem(item);
    }
}
