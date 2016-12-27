#include "savedserverswindow.h"
#include "ui_savedserverswindow.h"
#include "serverlistdelegate.h"
#include "serverlistmodel.h"

SavedServersWindow::SavedServersWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SavedServersWindow)
{
    ui->setupUi(this);

    ServerListModel* model = new ServerListModel(this);
    ui->servers->setModel(model);
    ui->servers->setItemDelegate(new ServerListDelegate(this));

    connect(ui->addServer, &QPushButton::clicked, this, &SavedServersWindow::addServer);
}

SavedServersWindow::~SavedServersWindow() {}

void SavedServersWindow::closeEvent(QCloseEvent* event)
{
    Q_UNUSED(event);
    ui->servers->model()->submit();
}

void SavedServersWindow::addServer()
{
    ui->servers->model()->insertRow(ui->servers->model()->rowCount());

    QModelIndex index = ui->servers->model()->index(ui->servers->model()->rowCount() - 1, 0);
    ui->servers->setCurrentIndex(index);
    ui->servers->edit(index);
}
