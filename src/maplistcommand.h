/*
 * maplistcommand.h
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

#ifndef MAPLISTCOMMAND_H
#define MAPLISTCOMMAND_H

#include <qrcon/qrconcommand.h>

class MapListCommand : public QRconCommand {
    Q_OBJECT

public:
    explicit MapListCommand(QObject* parent = nullptr);
    
    const QStringList& maps() const { return m_maps; }
    
private slots:
    void parseResponse();
    
private:
    QStringList m_maps;
    
};

#endif // MAPLISTCOMMAND_H
