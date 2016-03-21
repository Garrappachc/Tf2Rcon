/*
 * statuscommand.cpp
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

#include "statuscommand.h"
#include <QtCore>

StatusCommand::StatusCommand(QObject* parent): QRconCommand("status", parent)
{
    connect(this, &StatusCommand::finished, this, &StatusCommand::parseResponse);
}

void StatusCommand::parseIpAndPort(const QString& value)
{
    // value = i.e. 151.80.108.144:27015  (public ip: 151.80.108.144)
    QString first = value.split(" ").first();
    m_hostAdddress = first.split(":").first();
    QString port = first.split(":").last();
    if (!port.isEmpty())
        m_port = port.toUInt();
}

void StatusCommand::parseResponse()
{
    auto lines = body().split('\n');
    for (const QString& line: lines) {
        static QRegularExpression re("^([a-zA-Z\\/]+)\\s*:\\s(.+)$");
        auto match = re.match(line);
        if (match.hasMatch()) {
            QString key = match.captured(1);
            QString value = match.captured(2);
            
            if (key == "hostname") {
                m_hostName = value;
            } else if (key == "map") {
                m_currentLevel = value.split(" ").first();
            } else if (key == "udp/ip") {
                parseIpAndPort(value);
            } else if (key == "sourcetv") {
                m_hasSourceTv = true;
                parseSourceTvLine(value);
            }
        }
    }
}

void StatusCommand::parseSourceTvLine(const QString& line)
{
    QRegularExpression re("port (\\d+).*");
    auto match = re.match(line);
    if (match.hasMatch()) {
        m_sourceTvPort = match.captured(1).toUInt();
    }
}
