#ifndef SERVERSTATUSWIDGET_H
#define SERVERSTATUSWIDGET_H

#include <QWidget>

namespace Ui { class ServerStatusWidget; }

class ServerStatusWidget : public QWidget {
    Q_OBJECT

    Q_PROPERTY(bool connected READ isConnected WRITE setConnected NOTIFY connectedChanged)
    Q_PROPERTY(QString serverName READ serverName WRITE setServerName NOTIFY serverNameChanged)
    Q_PROPERTY(QString map READ map WRITE setMap NOTIFY mapChanged)

signals:
    void connectedChanged(bool connected);
    void serverNameChanged(const QString& serverName);
    void mapChanged(const QString& map);

public:
    explicit ServerStatusWidget(QWidget *parent = nullptr);
    virtual ~ServerStatusWidget();

    bool isConnected() const { return m_connected; }
    void setConnected(bool connected);

    const QString& serverName() const { return m_serverName; }
    void setServerName(const QString& serverName);

    const QString& map() const { return m_map; }
    void setMap(const QString& map);

private slots:
    void updateConnectedLabel(bool connected);

private:
    QScopedPointer<Ui::ServerStatusWidget> ui;
    bool m_connected = false;
    QString m_serverName;
    QString m_map;

};

#endif // SERVERSTATUSWIDGET_H
