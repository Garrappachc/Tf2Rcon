#include "commandlinewidget.h"
#include "ui_commandlinewidget.h"
#include <qrconcommand.h>
#include <QtWidgets>

CommandLineWidget::CommandLineWidget(QRconSession* rcon, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CommandLineWidget),
    m_rcon(rcon)
{
    ui->setupUi(this);

    connect(ui->execute, &QPushButton::clicked, this, &CommandLineWidget::executeCommand);
}

CommandLineWidget::~CommandLineWidget() {}

void CommandLineWidget::executeCommand()
{
    if (ui->command->text().isEmpty())
        return;

    ui->command->setEnabled(false);
    QString command = ui->command->text();
    QRconCommand* cmd = new QRconCommand(command, this);
    connect(cmd, &QRconCommand::finished, this, &CommandLineWidget::appendOutput);
    m_rcon->command(cmd);
}

void CommandLineWidget::appendOutput()
{
    QRconCommand* cmd = qobject_cast<QRconCommand*>(sender());
    Q_ASSERT(cmd);
    ui->console->appendPlainText(cmd->body());

    QTextCursor c = ui->console->textCursor();
    c.movePosition(QTextCursor::End);
    ui->console->setTextCursor(c);

    ui->command->clear();
    ui->command->setEnabled(true);
}
