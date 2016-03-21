/*
 * userlistcommand.h
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

#ifndef USERLISTCOMMAND_H
#define USERLISTCOMMAND_H

#include <qrcon/qrconcommand.h>
#include <QMap>

class UserListCommand : public QRconCommand {
    Q_OBJECT


public:
    explicit UserListCommand(QObject* parent = nullptr);
    
    int id(const QString& name) const;
    
    const QStringList& users() const { return m_users; }

private slots:
    void parseResponse();
    
private:
    QStringList m_users;
    QMap<QString, int> m_ids;
    
};

#endif // USERLISTCOMMAND_H
