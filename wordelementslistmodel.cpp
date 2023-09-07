#include "wordelementslistmodel.h"

WordElementsListModel::WordElementsListModel(QObject *parent)
    : QAbstractListModel{parent}
{

}

int WordElementsListModel::rowCount(QModelIndex const& parent) const
{
    return (parent.isValid()) ? 0 : mNames.count();
}

QVariant WordElementsListModel::data(QModelIndex const& index, int role) const
{
    if (!index.isValid())
        return {};
    switch (role)
    {
    case NameRole:
        return mNames.at(index.row());
    case ValueRole:
        return mValues.at(index.row());
    default:
        return {};
    }
}

QHash<int, QByteArray> WordElementsListModel::roleNames() const
{
    static QHash<int, QByteArray> const roles { {NameRole, "name"}, {ValueRole, "value"}, };
    return roles;
}

void WordElementsListModel::add(QString const& name, QString const& value)
{
    mNames.append(name);
    mValues.append(value);
}

void WordElementsListModel::clear()
{
    mNames.clear();
    mValues.clear();
}
