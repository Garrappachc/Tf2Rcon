#include "serverlistmodel.h"
#include <qrconserverconfig.h>
#include <QtCore>

ServerListModel::ServerListModel(QObject* parent)
    : QAbstractListModel(parent)
{
    read();
}

int ServerListModel::columnCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;

    return 3;
}

int ServerListModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;

    return m_servers.count();
}

QVariant ServerListModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
        case Qt::DisplayRole:
            switch (index.column()) {
                case 0: return m_servers.at(index.row()).hostName();
                case 1: return QString::number(m_servers.at(index.row()).port());
                case 2: return QString(m_servers.at(index.row()).password().length(), QChar(0x2022));
                default: return QVariant();
            }

        case Qt::EditRole:
            switch (index.column()) {
                case 0: return m_servers.at(index.row()).hostName();
                case 1: return m_servers.at(index.row()).port();
                case 2: return m_servers.at(index.row()).password();
            }

        default:
            return QVariant();
    }
}

QVariant ServerListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    switch (role) {
        case Qt::DisplayRole:
            if (orientation == Qt::Vertical) {
                return section + 1;
            } else {
                switch (section) {
                    case 0: return tr("Host name");
                    case 1: return tr("Port");
                    case 2: return tr("Password");
                    default: return QVariant();
                }
            }

        default:
            return QVariant();
    }
}

bool ServerListModel::insertRows(int row, int count, const QModelIndex& index)
{
    Q_UNUSED(index);

    beginInsertRows(QModelIndex(), row, row + count - 1);

    for (int i = 0; i < count; ++i) {
        m_servers.insert(row + i, QRconServerConfig());
    }

    endInsertRows();
    return true;
}

Qt::ItemFlags ServerListModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool ServerListModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {
        QRconServerConfig config = m_servers.at(index.row());
        switch (index.column()) {
            case 0: config.setHostName(value.toString()); break;
            case 1: config.setPort(value.toUInt()); break;
            case 2: config.setPassword(value.toString()); break;
            default: return false;
        }

        m_servers.replace(index.row(), config);
        emit dataChanged(index, index);
        return true;
    }

    return false;
}

bool ServerListModel::submit()
{
    save();
    return true;
}

void ServerListModel::read()
{
    m_servers.clear();

    QSettings s;
    if (s.contains("savedservers")) {
        QVariantMap map = s.value("savedservers").toMap();
        for (auto it = map.begin(); it != map.end(); ++it) {
            QString name = it.key();
            QRconServerConfig config = it.value().value<QRconServerConfig>();
            m_servers << config;
        }
    }
}

void ServerListModel::save()
{
    QVariantMap map;
    for (auto config: m_servers) {
        map.insert(QString("%1:%2").arg(config.hostName(), QString::number(config.port())), QVariant::fromValue(config));
    }

    QSettings s;
    s.setValue("savedservers", map);
}
