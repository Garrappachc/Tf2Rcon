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
    for (const QString& l: lines) {
        if (l.startsWith("<") || l.endsWith("users"))
            continue;
        
        auto seg = l.split(":");
        int id = seg[1].toInt();
        QString name = seg[2];
        if (name.startsWith('"'))
            name.remove(0, 1);
        if (name.endsWith('"'))
            name.remove(name.length() - 1, 1);
        
        m_users << name;
        m_ids.insert(name, id);
    }
}

