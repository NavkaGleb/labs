#pragma once

#include <QObject>
#include <QStyledItemDelegate>

class TaskListDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    // constructor / destructor
    TaskListDelegate(QObject* parent = nullptr);
    ~TaskListDelegate() = default;
    
    // public methods
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    void setEditorData(QWidget* editor, const QModelIndex& index) const override;
    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;
    void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
//    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};
