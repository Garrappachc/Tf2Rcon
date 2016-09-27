/*
 * tf2rconmainwindow.h
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

#ifndef TF2RCONMAINWINDOW_H
#define TF2RCONMAINWINDOW_H

#include <QMainWindow>
#include <QScopedPointer>
#include <qrconsession.h>

namespace Ui { class Tf2RconWindow; }
class SourceModPluginListCommand;
class StatusCommand;
class MapListCommand;
class UserListCommand;
class SavedServersWindow;

class Tf2RconMainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit Tf2RconMainWindow(QWidget* parent = nullptr);
    
    virtual ~Tf2RconMainWindow();
    
private slots:
    void showLoginDialog();
    void onAuthenticated();
    void onStatusUpdated();
    void onUsersUpdated();
    void onMapsUpdated();
    void changeLavel();
    void kickSelected();
    void fillConnectString();
    void fillSourceTvString();
    void execCommand();
    void updateStatus();
    void openSavedServersWindow();
    
private:
    QScopedPointer<Ui::Tf2RconWindow> ui;
    SourceModPluginListCommand* m_smPlugins;
    QRconSession* m_rcon;
    StatusCommand* m_status;
    MapListCommand* m_maps;
    UserListCommand* m_users;
    QScopedPointer<SavedServersWindow> m_savedServersWindow;

};

#endif // TF2RCONMAINWINDOW_H
