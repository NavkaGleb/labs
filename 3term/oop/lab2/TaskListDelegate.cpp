#include "TaskListDelegate.h"

#include <QCheckBox>
#include <QLineEdit>
#include <QPainter>
#include <QApplication>
#include <iostream>

// constructor
TaskListDelegate::TaskListDelegate(QObject* parent) : QStyledItemDelegate(parent) {

}

// public methods
QWidget* TaskListDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& /* option */,
                                        const QModelIndex& index) const {
    if (index.column() == 0)
        return new QLineEdit(parent);

    return nullptr;
}

void TaskListDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const {
    if (index.column() == 0) {
        auto* lineEdit = dynamic_cast<QLineEdit*>(editor);
        lineEdit->setText(index.data().toString());
    }
}
void TaskListDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const {
    if (index.column() == 0) {
        auto* lineEdit = dynamic_cast<QLineEdit*>(editor);
        model->setData(index, lineEdit->text());
    }
}

void TaskListDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& /* index */) const {
    editor->setGeometry(option.rect);
}
