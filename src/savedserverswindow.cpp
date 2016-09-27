#include "savedserverswindow.h"
#include "ui_savedserverswindow.h"

SavedServersWindow::SavedServersWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SavedServersWindow)
{
    ui->setupUi(this);
}

SavedServersWindow::~SavedServersWindow() {}
