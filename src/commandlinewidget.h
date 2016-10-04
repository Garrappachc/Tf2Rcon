#ifndef COMMANDLINEWIDGET_H
#define COMMANDLINEWIDGET_H

#include <qrconsession.h>
#include <QWidget>
#include <QScopedPointer>

namespace Ui { class CommandLineWidget; }

class CommandLineWidget : public QWidget {
    Q_OBJECT

public:
    explicit CommandLineWidget(QRconSession* rcon, QWidget *parent = nullptr);
    virtual ~CommandLineWidget();

private slots:
    /**
     * Executes command provided by user in the line edit.
     */
    void executeCommand();

    /**
     * Appends QRconCommand's output to the console.
     */
    void appendOutput();

private:
    QScopedPointer<Ui::CommandLineWidget> ui;
    QRconSession* m_rcon;
};

#endif // COMMANDLINEWIDGET_H
