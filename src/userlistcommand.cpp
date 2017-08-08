/*
 * userlistcommand.h
 * Copyright (C) 2016 Michał Garapich <michal@garapch.pl>
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

#include "userlistcommand.h"
#include <QtCore>

UserListCommand::UserListCommand(QObject* parent): QRconCommand("users", parent)
{
    connect(this, &UserListCommand::finished, this, &UserListCommand::parseResponse);
}

int UserListCommand::id(const QString& name) const
{
    return m_ids.value(name, -1);
}

void UserListCommand::parseResponse()
{
    m_users.clear();
    auto lines = body().split('\n', QString::SkipEmptyParts);

    QRegularExpression regex("(\\d+):(\\d+):\"([^\"]*)\"");

    for (const QString& l: lines) {
        auto match = regex.match(l);
        if (match.hasMatch()) {
            QString userName = match.captured(3);
            QString id = match.captured(2);

            m_users << userName;
            m_ids.insert(userName, id.toInt());
        }
    }
}
