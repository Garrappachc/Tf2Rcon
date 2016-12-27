#include "serverlistdelegate.h"
#include <QtWidgets>

ServerListDelegate::ServerListDelegate(QObject* parent) : QStyledItemDelegate(parent)
{

}

QWidget* ServerListDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    if (index.column() == 1) { // port
        QSpinBox* spinBox = new QSpinBox(parent);
        spinBox->setFrame(false);
        spinBox->setMinimum(1);
        spinBox->setMaximum(65536);
        return spinBox;
    } else {
        return QStyledItemDelegate::createEditor(parent, option, index);
    }

}

void ServerListDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    if (index.column() == 1) {
        int value = index.data(Qt::EditRole).toInt();
        QSpinBox* spinBox = qobject_cast<QSpinBox*>(editor);
        spinBox->setValue(value);
    } else {
        QStyledItemDelegate::setEditorData(editor, index);
    }
}

void ServerListDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    if (index.column() == 1) {
        QSpinBox* spinBox = qobject_cast<QSpinBox*>(editor);
        int value = spinBox->value();

        model->setData(index, static_cast<quint32>(value), Qt::EditRole);
    } else {
        QStyledItemDelegate::setModelData(editor, model, index);
    }
}

void ServerListDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    Q_UNUSED(index);

    editor->setGeometry(option.rect);
}
