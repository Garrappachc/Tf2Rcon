/*
 * statuscommand.h
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

#ifndef STATUSCOMMAND_H
#define STATUSCOMMAND_H

#include <qrconcommand.h>
#include <QHostAddress>

class StatusCommand : public QRconCommand {
    Q_OBJECT

public:
    explicit StatusCommand(QObject* parent = nullptr);
    
    const QString& hostName() const { return m_hostName; }
    const QString& currentLevel() const { return m_currentLevel; }
    const QHostAddress& hostAddress() const { return m_hostAdddress; }
    quint32 port() const { return m_port; }
    bool hasSourceTv() const { return m_hasSourceTv; }
    quint32 sourceTvPort() const { return m_sourceTvPort; }
    
private:
    void parseIpAndPort(const QString& value);
    
private slots:
    void parseResponse();
    void parseSourceTvLine(const QString& line);
    
private:
    QString m_hostName;
    QString m_currentLevel;
    QHostAddress m_hostAdddress;
    quint32 m_port;
    bool m_hasSourceTv;
    quint32 m_sourceTvPort;
    
};

#endif // STATUSCOMMAND_H
