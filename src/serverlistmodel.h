#ifndef SERVERLISTMODEL_H
#define SERVERLISTMODEL_H

#include <QAbstractListModel>

class QRconServerConfig;

class ServerListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit ServerListModel(QObject* parent = nullptr);

    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    bool insertRows(int row, int count, const QModelIndex& index = QModelIndex()) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;

public slots:
    bool submit() override;

private:
    void read();
    void save();

    QList<QRconServerConfig> m_servers;

};

#endif // SERVERLISTMODEL_H
