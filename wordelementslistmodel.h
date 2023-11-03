#ifndef WORDELEMENTSLISTMODEL_H
#define WORDELEMENTSLISTMODEL_H

#include <QAbstractListModel>

class WordElementsListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles {
        NameRole = Qt::UserRole,
        ValueRole,
    };

    explicit WordElementsListModel(QObject *parent = nullptr);

    int rowCount(QModelIndex const& parent) const override;
    QVariant data(QModelIndex const& index, int role = Qt::DisplayRole) const override;
    bool equals(WordElementsListModel const* other) const;

protected:
    QHash<int, QByteArray> roleNames() const override;

public slots:
    void add(QString const& name, QString const& value);
    void clear();

private:
    QList<QString> mNames;
    QList<QString> mValues;
};

#endif // WORDELEMENTSLISTMODEL_H
