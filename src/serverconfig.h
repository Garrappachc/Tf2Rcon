#ifndef SERVERCONFIG_H
#define SERVERCONFIG_H

#include <QString>

class ServerConfig {
public:
    ServerConfig();

    const QString& hostName() const { return m_hostName; }
    void setHostName(const QString& hostName);
    const QString& password() const { return m_password; }
    void setPassword(const QString& password);
    quint32 port() const { return m_port; }
    void setPort(quint32 port);

private:
    QString m_hostName;
    QString m_password;
    quint32 m_port;

};

#endif // SERVERCONFIG_H
