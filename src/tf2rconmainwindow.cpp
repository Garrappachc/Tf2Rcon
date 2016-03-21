/*
 * tf2rconmainwindow.cpp
 * Copyright (C) 2016 Michał Garapich <michal@garapich.pl>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#include "tf2rconmainwindow.h"
#include "ui_tf2rconmainwindow.h"
#include "logindialog.h"
#include "maplistcommand.h"
#include "statuscommand.h"
#include "userlistcommand.h"
#include <QtWidgets>
#include <functional>

Tf2RconMainWindow::Tf2RconMainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::Tf2RconWindow),
    m_rcon(new QRconSession(this))
{
    ui->setupUi(this);
    statusBar()->showMessage(tr("Not connected"));
    
    connect(ui->connectToServer, &QAction::triggered, this, &Tf2RconMainWindow::showLoginDialog);
    connect(ui->changelevel, &QPushButton::clicked, this, &Tf2RconMainWindow::changeLavel);
    connect(ui->kickSelected, &QPushButton::clicked, this, &Tf2RconMainWindow::kickSelected);
    connect(ui->maps, &QAbstractItemView::clicked, std::bind(&QPushButton::setEnabled, ui->changelevel, true));
    connect(ui->users, &QAbstractItemView::clicked, std::bind(&QPushButton::setEnabled, ui->kickSelected, true));
    
    connect(m_rcon, &QRconSession::authenticated, this, &Tf2RconMainWindow::onAuthenticated);
    
    connect(ui->connectString, &QLineEdit::selectionChanged, [this]() {
        if (ui->connectString->selectedText() != ui->connectString->text())
            ui->connectString->selectAll();
    });
}

Tf2RconMainWindow::~Tf2RconMainWindow() {}

void Tf2RconMainWindow::showLoginDialog()
{
    LoginDialog dialog(this);
    dialog.exec();
    
    if (dialog.result() == QDialog::Accepted) {
        m_rcon->setHostName(dialog.hostName());
        m_rcon->setPassword(dialog.password());
        m_rcon->setPort(dialog.port());
        m_rcon->authenticate();
    }
}

void Tf2RconMainWindow::onAuthenticated()
{
    statusBar()->showMessage(tr("Connected to %1").arg(m_rcon->hostName()));
    
    m_status = new StatusCommand(this);
    connect(m_status, &StatusCommand::finished, this, &Tf2RconMainWindow::onStatusUpdated);
    m_rcon->command(m_status);
    
    m_users = new UserListCommand(this);
    connect(m_users, &UserListCommand::finished, this, &Tf2RconMainWindow::onUsersUpdated);
    m_rcon->command(m_users);
    
    m_maps = new MapListCommand(this);
    connect(m_maps, &MapListCommand::finished, this, &Tf2RconMainWindow::onMapsUpdated);
    m_rcon->command(m_maps);
    
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout,
            std::bind(static_cast<void (QRconSession::*)(QRconCommand*)>(&QRconSession::command), m_rcon, m_status)); // refresh status
    connect(timer, &QTimer::timeout,
            std::bind(static_cast<void (QRconSession::*)(QRconCommand*)>(&QRconSession::command), m_rcon, m_users)); // refresh users
    
    timer->start(1000 * 60); // 1 minute
}

void Tf2RconMainWindow::onStatusUpdated()
{
    ui->hostName->setText(m_status->hostName());
    ui->currentLevel->setText(m_status->currentLevel());
    
    QRconCommand* cmd = new QRconCommand("sv_password", this);
    connect(cmd, &QRconCommand::finished, this, &Tf2RconMainWindow::fillConnectString);
    m_rcon->command(cmd);
}

void Tf2RconMainWindow::onUsersUpdated()
{
    if (ui->users->model())
        ui->users->model()->deleteLater();
    
    ui->users->clearSelection();
    ui->kickSelected->setEnabled(false);
    
    QAbstractItemModel* model = new QStringListModel(m_users->users(), this);
    ui->users->setModel(model);
}

void Tf2RconMainWindow::onMapsUpdated()
{
    if (ui->maps->model())
        ui->maps->model()->deleteLater();
    
    ui->changelevel->setEnabled(false);
    ui->maps->clearSelection();
    
    QAbstractItemModel* model = new QStringListModel(m_maps->maps(), this);
    ui->maps->setModel(model);
}

void Tf2RconMainWindow::changeLavel()
{
    QModelIndex index = ui->maps->currentIndex();
    QString map = index.data().toString();
    
    QRconCommand* cmd = new QRconCommand("changelevel " + map, this);
    connect(cmd, &QRconCommand::finished, [cmd, this]() {
        this->m_rcon->command(this->m_status);
        cmd->deleteLater();
    });
    
    m_rcon->command(cmd);
    ui->changelevel->setEnabled(false);
    ui->maps->clearSelection();
}

void Tf2RconMainWindow::kickSelected()
{
    QModelIndex index = ui->users->currentIndex();
    QString userName = index.data().toString();
    int id = m_users->id(userName);
    
    QRconCommand* cmd = new QRconCommand(QStringLiteral("kickid %1").arg(QString::number(id)), this);
    connect(cmd, &QRconCommand::finished, [cmd, this]() {
        this->m_rcon->command(this->m_status);
        cmd->deleteLater();
    });
    
    m_rcon->command(cmd);
    ui->kickSelected->setEnabled(false);
    ui->users->clearSelection();
}

void Tf2RconMainWindow::fillConnectString()
{
    QRconCommand* cmd = qobject_cast<QRconCommand*>(sender()); // sv_password
    Q_ASSERT(cmd);
    
    QRegularExpression re("^\\\"sv_password\\\"\\s=\\s\\\"(.[^\\\"]+)\\\".*$");
    
    auto lines = cmd->body().split('\n');
    for (auto l: lines) {
        auto match = re.match(l);
        if (match.hasMatch()) {
            QString password = match.captured(1);
            QString connectString = QStringLiteral("connect %1:%2").arg(
                m_rcon->hostName(), QString::number(m_rcon->port()));
            
            if (!password.isEmpty()) {
                connectString += "; password " + password;
            }
            
            ui->connectString->setText(connectString);
            ui->connectString->setEnabled(true);
            
            break;
        }
    }
}

void Tf2RconMainWindow::fillSourceTvString()
{

}
