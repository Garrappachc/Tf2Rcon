#ifndef SAVEDSERVERSWINDOW_H
#define SAVEDSERVERSWINDOW_H

#include <QWidget>
#include <QScopedPointer>

namespace Ui { class SavedServersWindow; }

class SavedServersWindow : public QWidget {
    Q_OBJECT

public:
    explicit SavedServersWindow(QWidget *parent = nullptr);
    ~SavedServersWindow();

private:
    QScopedPointer<Ui::SavedServersWindow> ui;
};

#endif // SAVEDSERVERSWINDOW_H
