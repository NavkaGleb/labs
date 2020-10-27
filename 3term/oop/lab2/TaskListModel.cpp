#include "TaskListModel.h"

#include <iostream>
#include <algorithm>

#include <QCheckBox>

// constructor
TaskListModel::TaskListModel(QObject* parent)
    : QAbstractListModel(parent) {
}

// accessors
int TaskListModel::rowCount(const QModelIndex& /* index */) const { return this->_tasks.count(); }

int TaskListModel::columnCount(const QModelIndex& /* index */) const { return 2; }

QVariant TaskListModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid())
        return {};

    int row = index.row();
    int column = index.column();
    const auto* task = this->_tasks.at(row);

    switch(role) {
        case Qt::DisplayRole:
            switch (column) {
                case 0:
                    return task->name();
                case 1:
                    return task->date() ? task->date()->toString("MMM dd") : "no date";
            }
        case Qt::TextAlignmentRole:
            switch (column) {
                case 0: return int(Qt::AlignLeft | Qt::AlignVCenter);
                case 1: return int(Qt::AlignRight | Qt::AlignVCenter);
            }
        case Qt::CheckStateRole:
            if (column == 0)
                return task->done() ? Qt::Checked : Qt::Unchecked;
        default:
            return {};
    }

    return {};
}

Qt::ItemFlags TaskListModel::flags(const QModelIndex& index) const {
//    if (index.column() == 0)
//        return Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEditable | Qt::ItemIsEnabled;


    return Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable;
}

// modifiers
bool TaskListModel::insertRows(int row, int count, const QModelIndex& parent) {
    for (int i = 0; i < count; ++i)
        this->_tasks.append(nullptr);

    return true;
}

bool TaskListModel::setData(const QModelIndex& index, const QVariant& value, int role) {
//    if (role != Qt::EditRole)
//        return false;

    auto*& task = this->_tasks[index.row()];

    this->beginResetModel();

    std::cout << value.toBool() << std::endl;
    std::cout << value.toString().toStdString() << std::endl;

    if (role == Qt::EditRole) {
        if (index.column() == 0) {
            task->setName(value.toString());
            return true;
        }
    } else if (role == Qt::CheckStateRole) {
        bool done = value.toBool();
        std::cout << "bool value = " << done << std::endl;
        task->setDone(done);
        QModelIndex topLeft = index;
        QModelIndex bottomRight = index;
        emit dataChanged(topLeft, bottomRight);
    }

    this->endResetModel();
    this->sort();

    return true;
}

void TaskListModel::setItem(Ng::Task* task) {
    this->beginResetModel();

    int row = this->rowCount();
    this->insertRow(1);
    this->_tasks.replace(row, task);

    this->endResetModel();
    this->sort();
}

void TaskListModel::sort() {
    std::sort(this->_tasks.begin(), this->_tasks.end(), [](const Ng::Task* left, const Ng::Task* right) {
        return *left < *right;
    });
    emit dataChanged(index(0, 0), index(this->_tasks.count(), 2));
    std::cout << "sortring" << std::endl;
}
