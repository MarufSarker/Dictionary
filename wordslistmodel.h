#ifndef WORDSLISTMODEL_H
#define WORDSLISTMODEL_H

#include <QAbstractListModel>
#include <QSqlDatabase>
#include "wordelementslistmodel.h"

class WordsListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles {
        WordElementsRole = Qt::UserRole,
    };

    explicit WordsListModel(QObject *parent = nullptr);

    int rowCount(QModelIndex const& parent) const override;
    QVariant data(QModelIndex const& index, int role = Qt::DisplayRole) const override;

protected:
    QHash<int, QByteArray> roleNames() const override;

public slots:
    void search(QString const& queries);

private:
    QList<WordElementsListModel*> mData;
    QSqlDatabase mDb;
};

#endif // WORDSLISTMODEL_H
