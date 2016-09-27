/*
 * logindialog.h
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

#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QComboBox>
#include <QDialog>
#include <QLineEdit>
#include <QSpinBox>

class LoginDialog : public QDialog {
    Q_OBJECT

public:
    explicit LoginDialog(QWidget* parent = nullptr);
    
    virtual ~LoginDialog();
    
    QString hostName();
    QString password();
    quint32 port();

private slots:
    void activateServerConfig(int index);
    
private:
    void loadServers();

    QLineEdit* m_hostName;
    QLineEdit* m_password;
    QSpinBox* m_port; 
    QComboBox* m_servers;

};

#endif // LOGINDIALOG_H
