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
#include "commandlinewidget.h"
#include "logindialog.h"
#include "maplistcommand.h"
#include "savedserverswindow.h"
#include "serverstatuswidget.h"
#include "sourcemodpluginlistcommand.h"
#include "statuscommand.h"
#include "userlistcommand.h"
#include <qcvarcommand.h>
#include <QtWidgets>
#include <functional>

Tf2RconMainWindow::Tf2RconMainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::Tf2RconWindow),
    m_rcon(new QRconSession(this))
{
    ui->setupUi(this);
    statusBar()->showMessage(tr("Not connected"));

    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(this);
    ui->maps->setModel(proxyModel);
    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);

    connect(ui->mapFilter, &QLineEdit::textChanged, proxyModel, qOverload<const QString&>(&QSortFilterProxyModel::setFilterRegExp));
    connect(ui->connectToServer, &QAction::triggered, this, &Tf2RconMainWindow::showLoginDialog);
    connect(ui->actionSavedServers, &QAction::triggered, this, &Tf2RconMainWindow::openSavedServersWindow);
    connect(ui->changelevel, &QPushButton::clicked, this, &Tf2RconMainWindow::changeLavel);
    connect(ui->kickSelected, &QPushButton::clicked, this, &Tf2RconMainWindow::kickSelected);
    connect(ui->maps, &QAbstractItemView::clicked, std::bind(&QPushButton::setEnabled, ui->changelevel, true));
    connect(ui->users, &QAbstractItemView::clicked, std::bind(&QPushButton::setEnabled, ui->kickSelected, true));
    connect(ui->command, &QPushButton::clicked, this, &Tf2RconMainWindow::execCommand);
    
    connect(m_rcon, &QRconSession::authenticated, this, &Tf2RconMainWindow::onAuthenticated);
    connect(m_rcon, &QRconSession::error, this, &Tf2RconMainWindow::onSessionError);
    
    connect(ui->connectString, &QLineEdit::selectionChanged, [this]() {
        if (ui->connectString->selectedText() != ui->connectString->text())
            ui->connectString->selectAll();
    });

    connect(ui->sourceTvConnectString, &QLineEdit::selectionChanged, [this]() {
        if (ui->sourceTvConnectString->selectedText() != ui->sourceTvConnectString->text())
            ui->sourceTvConnectString->selectAll();
    });

    setEnabled(false);
}

Tf2RconMainWindow::~Tf2RconMainWindow() {}

void Tf2RconMainWindow::refresh()
{
    m_rcon->command(m_status);
    m_rcon->command(m_users);
}

void Tf2RconMainWindow::showEvent(QShowEvent *event)
{
    QTimer::singleShot(50, this, &Tf2RconMainWindow::showLoginDialog);
    Q_UNUSED(event);
}

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
    setEnabled(true);
    statusBar()->showMessage(tr("Connected to %1").arg(m_rcon->hostName()));
    ui->serverStatus->setConnected(true);
    
    m_status = new StatusCommand(this);
    connect(m_status, &StatusCommand::finished, this, &Tf2RconMainWindow::onStatusUpdated);
    m_rcon->command(m_status);
    
    m_users = new UserListCommand(this);
    connect(m_users, &UserListCommand::finished, this, &Tf2RconMainWindow::onUsersUpdated);
    m_rcon->command(m_users);
    
    m_maps = new MapListCommand(this);
    connect(m_maps, &MapListCommand::finished, this, &Tf2RconMainWindow::onMapsUpdated);
    m_rcon->command(m_maps);

    m_smPlugins = new SourceModPluginListCommand(this);
    m_rcon->command(m_smPlugins);
    
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout,
            std::bind(&QRconSession::command, m_rcon, m_status)); // refresh status
    connect(timer, &QTimer::timeout,
            std::bind(&QRconSession::command, m_rcon, m_users)); // refresh users
    
    timer->start(1000 * 60); // 1 minute
}

void Tf2RconMainWindow::onSessionError()
{
    ui->serverStatus->setConnected(false);
    setEnabled(false);
}

void Tf2RconMainWindow::onStatusUpdated()
{
    ui->serverStatus->setServerName(m_status->hostName());
    ui->serverStatus->setMap(m_status->currentLevel());
    
    QCvarCommand* cmd = new QCvarCommand("sv_password", this);
    connect(cmd, &QRconCommand::finished, this, &Tf2RconMainWindow::fillConnectString);
    m_rcon->command(cmd);

    cmd = new QCvarCommand("tv_password", this);
    connect(cmd, &QRconCommand::finished, this, &Tf2RconMainWindow::fillSourceTvString);
    m_rcon->command(cmd);
    
    ui->command->setEnabled(true);
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
    ui->changelevel->setEnabled(false);
    ui->maps->clearSelection();

    QSortFilterProxyModel* proxyModel = qobject_cast<QSortFilterProxyModel*>(ui->maps->model());
    QStringListModel* model = qobject_cast<QStringListModel*>(proxyModel->sourceModel());
    if (model == nullptr) {
        model = new QStringListModel(this);
        proxyModel->setSourceModel(model);
    }

    model->setStringList(m_maps->maps());
}

void Tf2RconMainWindow::changeLavel()
{
    QModelIndex index = ui->maps->currentIndex();
    QString map = index.data().toString();

    if (m_smPlugins->plugins().contains(QStringLiteral("Nextmap"))) {
        // use SourceMod if available
        QRconCommand* cmd = new QRconCommand(QStringLiteral("sm_map %1").arg(map), this);
        connect(cmd, &QRconCommand::finished, this, std::bind(&QRconSession::command, m_rcon, m_status));
        m_rcon->command(cmd);
    } else {
        QRconCommand* cmd = new QRconCommand(QStringLiteral("changelevel %1").arg(map), this);
        connect(cmd, &QRconCommand::finished, this, std::bind(&QRconSession::command, m_rcon, m_status));
        m_rcon->command(cmd);
    }

    ui->changelevel->setEnabled(false);
    ui->maps->clearSelection();
}

void Tf2RconMainWindow::kickSelected()
{
    QModelIndex index = ui->users->currentIndex();
    QString userName = index.data().toString();
    int id = m_users->id(userName);
    
    QRconCommand* cmd = new QRconCommand(QStringLiteral("kickid %1").arg(QString::number(id)), this);
    connect(cmd, &QRconCommand::finished, std::bind(&QRconSession::command, m_rcon, m_users));
    connect(cmd, &QRconCommand::finished, &QObject::deleteLater);
    
    m_rcon->command(cmd);
    ui->kickSelected->setEnabled(false);
    ui->users->clearSelection();
}

void Tf2RconMainWindow::fillConnectString()
{
    QCvarCommand* cmd = qobject_cast<QCvarCommand*>(sender()); // sv_password
    Q_ASSERT(cmd);
    Q_ASSERT(cmd->command() == "sv_password");
    
    QString connectString = QStringLiteral("connect %1:%2").arg(
        m_rcon->hostName(), QString::number(m_status->port()));
    
    auto password = cmd->value();
    if (!password.isEmpty()) {
        connectString += "; password " + password;
    }
            
    ui->connectString->setText(connectString);
    ui->connectString->setEnabled(true);

    cmd->deleteLater();
}

void Tf2RconMainWindow::fillSourceTvString()
{
    QCvarCommand* cmd = qobject_cast<QCvarCommand*>(sender()); // tv_password
    Q_ASSERT(cmd);
    Q_ASSERT(cmd->command() == "tv_password");

    QString connectString = QStringLiteral("connect %1:%2").arg(
        m_rcon->hostName(), QString::number(m_status->sourceTvPort()));

    auto password = cmd->value();
    if (!password.isEmpty()) {
        connectString += "; password " + password;
    }

    ui->sourceTvConnectString->setText(connectString);
    ui->sourceTvConnectString->setEnabled(true);

    cmd->deleteLater();
}

void Tf2RconMainWindow::execCommand()
{
    if (m_commandLineWindow.isNull())
        m_commandLineWindow.reset(new CommandLineWidget(m_rcon));

    m_commandLineWindow->show();
}

void Tf2RconMainWindow::openSavedServersWindow()
{
    if (!m_savedServersWindow) {
        m_savedServersWindow.reset(new SavedServersWindow);
    }

    m_savedServersWindow->show();
}
