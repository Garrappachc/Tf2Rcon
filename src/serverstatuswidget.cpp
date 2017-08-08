#include "serverstatuswidget.h"
#include "ui_serverstatuswidget.h"

ServerStatusWidget::ServerStatusWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ServerStatusWidget)
{
    ui->setupUi(this);

    connect(this, &ServerStatusWidget::connectedChanged, &ServerStatusWidget::updateConnectedLabel);
    connect(this, &ServerStatusWidget::serverNameChanged, ui->serverName, &QLabel::setText);
    connect(this, &ServerStatusWidget::mapChanged, ui->map, &QLabel::setText);
}

ServerStatusWidget::~ServerStatusWidget()
{

}

void ServerStatusWidget::setConnected(bool connected)
{
    if (connected != m_connected) {
        m_connected = connected;
        emit connectedChanged(m_connected);
    }
}

void ServerStatusWidget::setServerName(const QString &serverName)
{
    m_serverName = serverName;
    emit serverNameChanged(m_serverName);
}

void ServerStatusWidget::setMap(const QString &map)
{
    m_map = map;
    emit mapChanged(m_map);
}

void ServerStatusWidget::updateConnectedLabel(bool connected)
{
    ui->connectionStatus->setText(connected ? tr("Connected") : tr("Not connected"));
}
