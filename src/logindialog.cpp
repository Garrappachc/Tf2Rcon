/*
 * logindialog.cpp
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

#include "logindialog.h"
#include <qrconserverconfig.h>
#include <QtWidgets>

LoginDialog::LoginDialog(QWidget* parent) : QDialog(parent)
{
    QVBoxLayout* layout = new QVBoxLayout;
    
    m_hostName = new QLineEdit;
    m_hostName->setPlaceholderText(tr("Server address"));
    layout->addWidget(m_hostName);
    
    m_password = new QLineEdit;
    m_password->setPlaceholderText(tr("Password"));
    m_password->setEchoMode(QLineEdit::Password);
    layout->addWidget(m_password);
    
    m_port = new QSpinBox;
    m_port->setMaximum(65535);
    m_port->setValue(27015);
    layout->addWidget(m_port);
    
    QPushButton* login = new QPushButton(tr("Login"));
    connect(login, &QPushButton::clicked, this, &QDialog::accept);
    layout->addWidget(login);

    m_servers = new QComboBox;
    m_servers->addItem(QString()); // empty config by default
    connect(m_servers, SIGNAL(activated(int)), this, SLOT(activateServerConfig(int)));
    layout->addWidget(m_servers);

    setLayout(layout);

    loadServers();
}

LoginDialog::~LoginDialog() {}

QString LoginDialog::hostName()
{
    return m_hostName->text();
}

QString LoginDialog::password()
{
    return m_password->text();
}

quint32 LoginDialog::port()
{
    return static_cast<quint32>(m_port->value());
}

void LoginDialog::activateServerConfig(int index)
{
    QVariant data = m_servers->itemData(index);
    if (data.isValid()) {
        QRconServerConfig config = data.value<QRconServerConfig>();
        m_hostName->setText(config.hostName());
        m_password->setText(config.password());
        m_port->setValue(config.port());
    }
}

void LoginDialog::loadServers()
{
    QSettings s;
    if (s.contains("savedservers")) {
        QVariantMap map = s.value("savedservers").toMap();
        for (auto it = map.begin(); it != map.end(); ++it) {
            QString name = it.key();
            QRconServerConfig config = it.value().value<QRconServerConfig>();
            m_servers->addItem(name, QVariant::fromValue(config));
        }
    }
}
