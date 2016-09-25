#ifndef SOURCEMODPLUGINLISTCOMMAND_H
#define SOURCEMODPLUGINLISTCOMMAND_H

#include <qrconcommand.h>
#include <QList>
#include <QString>

class SourceModPluginListCommand : public QRconCommand {
    Q_OBJECT

public:
    SourceModPluginListCommand(QObject* parent = nullptr);

    const QList<QString>& plugins() const { return m_plugins; }

private slots:
    void parseResponse();

private:
    QList<QString> m_plugins;
};

#endif // SOURCEMODPLUGINLISTCOMMAND_H
