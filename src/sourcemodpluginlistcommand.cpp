#include <QtCore>
#include "sourcemodpluginlistcommand.h"

SourceModPluginListCommand::SourceModPluginListCommand(QObject* parent) :
        QRconCommand("sm plugins list", parent)
{
    connect(this, &QRconCommand::finished, this, &SourceModPluginListCommand::parseResponse);
}

void SourceModPluginListCommand::parseResponse()
{
    auto lines = body().split('\n');
    for (const QString& line: lines) {
        static QRegularExpression re("\\d+\\s\"(.[^\"]+)\"");
        auto match = re.match(line);
        if (match.hasMatch()) {
            QString plugin = match.captured(1);
            if (!plugin.isEmpty()) {
                m_plugins.append(plugin);
            }
        }
    }

    qDebug() << "Found" << m_plugins.size() << "SourceMod plugins";
}
