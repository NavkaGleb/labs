#include "MainWindow.h"
#include "ui_MainWindow.h"

// constructor / destructor
MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

// private slots
void MainWindow::on_newTask_editingFinished() {
    auto& newTask = *(ui->newTask);

    std::cout << newTask.text().toStdString() << std::endl;
    newTask.clear();
}
