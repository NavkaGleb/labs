#pragma once

#include <QAbstractListModel>

#include "Task.h"

class TaskListModel : public QAbstractListModel {
    Q_OBJECT
public:
    // constructor / destructor
    explicit TaskListModel(QObject* parent = nullptr);
    virtual ~TaskListModel() = default;

    // accessors
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    Ng::Task* getItem(const QModelIndex& index) const;

    // modifiers
    bool insertRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
    void setItem(Ng::Task* task);

    // public methods
    void sort();
    void update();

private:
    // fields
    QList<Ng::Task*> _tasks;
};
