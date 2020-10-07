#pragma once

#include <iostream>
#include <QMainWindow>

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
    void on_newTask_editingFinished();

private:
    Ui::MainWindow* ui;
};
