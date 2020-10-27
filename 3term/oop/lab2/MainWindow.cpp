#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QMessageBox>
#include <QPushButton>
#include <QLineEdit>
#include <QModelIndex>
#include <QTreeWidget>
#include <iostream>

// constructor / destructor
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), _ui(new Ui::MainWindow), _currentTask(new Ng::Task) {
    this->_ui->setupUi(this);
    this->setWindowTitle("to do list");
    this->_calendarForm = new CalendarForm(this);
    this->_calendarForm->setModal(true);

    this->_lists["personal"];
    this->_lists["work"];

    this->_taskListModel = new TaskListModel(this);
    this->_taskListDelegate = new TaskListDelegate(this);

    this->initListsContainer();
    this->initTasksContainer();
    this->taskFormSetEnabled(false);
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

    if (this->_currentTask->date())
        this->_ui->dateButton->setText(QString::number(this->_currentTask->date()->day()));
}

void MainWindow::on_priority_activated(int index) {
    if (!this->_currentTask)
        this->_currentTask = new Ng::Task;

    this->_currentTask->setPriority(static_cast<Ng::Task::Priority>(index));
}

void MainWindow::on_newTask_returnPressed() {
    if (this->_ui->newTask->text().isEmpty())
        return;

    if (!this->_currentTask)
        this->_currentTask = new Ng::Task;

    this->_currentTask->setName(this->_ui->newTask->text());
    this->_lists[this->_currentList].append(this->_currentTask);

    this->addTask(this->_currentTask);
    this->clear();
}

void MainWindow::on_tasksContainer_clicked(const QModelIndex& index) {
    this->_currentTask = this->_taskListModel->getItem(index);

    if (!this->_currentTask)
        return;

    this->taskFormSetEnabled(true);

    if (this->_currentTask->time())
        this->_ui->taskTime->setTime(*this->_currentTask->time());

    if (this->_currentTask->date())
        this->_ui->taskDate->setDate(*this->_currentTask->date());

    this->_ui->taskPriority->setCurrentIndex(static_cast<int>(this->_currentTask->priority()));
    this->_ui->taskName->setText(this->_currentTask->name());
    this->_ui->taskDescription->setText(this->_currentTask->description());
}

void MainWindow::on_taskTime_editingFinished() {
    this->_ui->taskTime->clearFocus();
    this->_currentTask->setTime(this->_ui->taskTime->time());
    this->_taskListModel->update();
}

void MainWindow::on_taskName_returnPressed() {
    this->_ui->taskName->clearFocus();
    this->_currentTask->setName(this->_ui->taskName->text());
    this->_taskListModel->update();
}

void MainWindow::on_taskDate_editingFinished() {
    this->_ui->taskDate->clearFocus();
    this->_currentTask->setDate(this->_ui->taskDate->date());
    this->_taskListModel->update();
}

void MainWindow::on_taskPriority_activated(int index) {
    this->_ui->taskPriority->clearFocus();
    this->_currentTask->setPriority(static_cast<Ng::Task::Priority>(index));
    this->_taskListModel->update();
}

void MainWindow::on_taskDescription_textChanged() {
    this->_currentTask->setDescription(this->_ui->taskDescription->toPlainText());
}

void MainWindow::on_newTask_textChanged(const QString& /* arg */){
    if (this->_currentTask)
        this->_currentTask = nullptr;
}

void MainWindow::on_listsContainer_itemClicked(QTreeWidgetItem* item, int /* column */) {
    this->_currentList = item->data(0, Qt::DisplayRole).toString();
    this->updateTasks();
}

void MainWindow::on_addList_clicked() {
    this->_ui->listsContainer->editItem(this->_ui->listsContainer->topLevelItem(0), 0);
}

// private methods
void MainWindow::initListsContainer() {
    this->_ui->listsContainer->setHeaderHidden(true);

    for (int i = 0; i < this->_lists.keys().count(); ++i) {
        auto* item = new QTreeWidgetItem(this->_ui->listsContainer);

        item->setText(0, this->_lists.keys()[i]);
        this->_ui->listsContainer->addTopLevelItem(item);
    }

    this->_ui->listsContainer->setCurrentItem(this->_ui->listsContainer->topLevelItem(0), 0, QItemSelectionModel::Select);
    this->_currentList = "personal";
}

void MainWindow::initTasksContainer() {    
    this->_ui->tasksContainer->setModel(this->_taskListModel);
    this->_ui->tasksContainer->verticalHeader()->setVisible(false);
    this->_ui->tasksContainer->horizontalHeader()->setVisible(false);
    this->_ui->tasksContainer->setItemDelegate(this->_taskListDelegate);
    this->_ui->tasksContainer->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->_ui->tasksContainer->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->_ui->tasksContainer->setEditTriggers(QAbstractItemView::AnyKeyPressed | QAbstractItemView::DoubleClicked);
    this->_ui->tasksContainer->setSortingEnabled(true);
    this->_ui->tasksContainer->setFocusPolicy(Qt::NoFocus);
    setStyleSheet("QTableView::item:selected {background: rgba(0, 0, 0, 1%);}");

    for (const auto& task : this->_lists["today"])
        this->addTask(task);
}

void MainWindow::addTask(Ng::Task* task) { this->_taskListModel->setItem(task); }

void MainWindow::clear() {
    this->_ui->newTask->clear();
    this->_ui->dateButton->setText("date");
    this->_ui->priority->setCurrentIndex(0);
    this->_currentTask = nullptr;
    this->_calendarForm->clear();
}

void MainWindow::taskFormSetEnabled(bool enabled) {
    this->_ui->taskTime->setEnabled(enabled);
    this->_ui->taskDate->setEnabled(enabled);
    this->_ui->taskPriority->setEnabled(enabled);
    this->_ui->taskName->setEnabled(enabled);
    this->_ui->taskDescription->setEnabled(enabled);
}

void MainWindow::resizeEvent(QResizeEvent* event) {
    this->_ui->tasksContainer->setColumnWidth(0, this->width() * 0.75);
    this->_ui->tasksContainer->horizontalHeader()->setStretchLastSection(true);
    QMainWindow::resizeEvent(event);
}

void MainWindow::updateTasks() {
    this->_taskListModel->clear();
    this->_taskListModel->update();

    for (auto& task : this->_lists[this->_currentList])
        this->_taskListModel->setItem(task);
}
