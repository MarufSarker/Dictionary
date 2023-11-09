#include "wordslistmodel.h"
#include <QtSql>

WordsListModel::WordsListModel(QObject *parent)
    : QAbstractListModel{parent}
{
    QString dbPath = {};

    {
        QString loc = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
        QDir dir(loc);

        if (!dir.exists())
            dir.mkpath(loc);

        QString src = ":/assets/databases/dictionary.db";
        QString dst = loc + "/dictionary.db";
        QFile srcFile (src);
        QFile dstFile (dst);

        qDebug() << "[Source]" << src << srcFile.fileName();
        qDebug() << "[Destination]" << dst << dstFile.fileName();

        if (!dstFile.exists())
        {
            qDebug() << dstFile.fileName() << "does not exists";
            if (!srcFile.copy(dst))
            {
                dbPath = {};
                qDebug() << "Failed to copy" << srcFile.fileName() << "to" << dstFile.fileName();
                qDebug() << srcFile.error() << srcFile.errorString();
                return;
            }
            qDebug() << "Copied" << srcFile.fileName() << "to" << dst;
            dbPath = dst;
        }
        else if (QFileInfo(dst).lastModified().daysTo(QDateTime::currentDateTime()) < 1)
        {
            qDebug() << dst << "exists and recent";
            dbPath = dst;
        }
        else
        {
            if (!dstFile.remove())
            {
                dbPath = {};
                qDebug() << "Failed to remove" << dstFile.fileName();
                qDebug() << dstFile.error() << dstFile.errorString();
                return;
            }
            qDebug() << "Removed" << dstFile.fileName();
            if (!srcFile.copy(dst))
            {
                dbPath = {};
                qDebug() << "Failed to copy" << srcFile.fileName() << "to" << dstFile.fileName();
                qDebug() << srcFile.error() << srcFile.errorString();
                return;
            }
            qDebug() << "Copied" << srcFile.fileName() << "to" << dst;
            dbPath = dst;
        }


//        if (QFile::exists(dst))
//        {
//            if (QFileInfo(dst).lastModified().daysTo(QDateTime::currentDateTime()) < 1)
//                dbPath = dst;
//            else if (QFile::remove(dst) && QFile::copy(src, dst))
//                dbPath = dst;
//            else
//            {
//                dbPath = {};
//                qDebug() << "Failed to find existing cache file or copy file.";
//            }
//        }
//        else
//        {
//            if (QFile::copy(src, dst))
//                dbPath = dst;
//            else
//            {
//                dbPath = {};
//                qDebug() << "Failed to copy file";
//            }
//        }
    }

    qDebug() << "[Database]" << dbPath;

    if (dbPath.isEmpty())
        return;

    mDb = QSqlDatabase::addDatabase("QSQLITE");
    mDb.setConnectOptions("QSQLITE_OPEN_READONLY");
    mDb.setDatabaseName(dbPath);

    if (!mDb.open())
        qDebug() << mDb.lastError();
}

int WordsListModel::rowCount(QModelIndex const& parent) const
{
    return (parent.isValid()) ? 0 : mData.count();
}

QVariant WordsListModel::data(QModelIndex const& index, int role) const
{
    if (!index.isValid())
        return {};
    switch (role)
    {
    case WordElementsRole:
        return QVariant::fromValue(mData.at(index.row()));
    default:
        return {};
    }
}

QHash<int, QByteArray> WordsListModel::roleNames() const
{
    static QHash<int, QByteArray> const roles { {WordElementsRole, "wordElements"}, };
    return roles;
}

void WordsListModel::search(QString const& queries)
{
    auto queryDb = [](QObject* parent, QSqlQuery& query, QList<WordElementsListModel*>& data)
    {
        if (!query.exec())
        {
            qDebug() << query.lastError();
            query.finish();
            return;
        }

        int idEn = query.record().indexOf("en");
        int idBn = query.record().indexOf("bn");
        int idEnSynonyms = query.record().indexOf("en_synonyms");
        int idBnSynonyms = query.record().indexOf("bn_synonyms");
        int idBnPronunciations = query.record().indexOf("bn_pronunciations");
        int idEnLemma = query.record().indexOf("en_lemma");
        int idExplanation = query.record().indexOf("explanation");
        int idExample = query.record().indexOf("example");
        int idEnSentences = query.record().indexOf("en_sentences");

        while (query.next())
        {
            WordElementsListModel* elem = new WordElementsListModel(parent);
            elem->add("English", query.value(idEn).toString());
            elem->add("Bengali", query.value(idBn).toString());
            elem->add("English Synonyms", query.value(idEnSynonyms).toString());
            elem->add("Bengali Synonyms", query.value(idBnSynonyms).toString());
            elem->add("Bengali Pronunciations", query.value(idBnPronunciations).toString());
            elem->add("English Lemma", query.value(idEnLemma).toString());
            elem->add("Explanation", query.value(idExplanation).toString());
            elem->add("Example", query.value(idExample).toString());
            elem->add("English Sentences", query.value(idEnSentences).toString());
            data.append(elem);
        }
        query.finish();
    };

    beginRemoveRows(QModelIndex(), 0, mData.size() - 1);
    qDeleteAll(mData.begin(), mData.end());
    mData.clear();
    endRemoveRows();

    if (queries.isEmpty() || !mDb.isOpen())
        return;

    QList<WordElementsListModel*> tmpData;
    QSqlQuery query;

    query.prepare("SELECT * FROM dictionary WHERE [bn] = (:bn) OR [en] = (:en) LIMIT 10");
    query.bindValue(":bn", queries);
    query.bindValue(":en", queries);
    queryDb(this, query, tmpData);

    query.prepare("SELECT * FROM dictionary WHERE [bn] LIKE (:bn) OR [en] LIKE (:en) LIMIT 10");
    query.bindValue(":bn", queries + "%");
    query.bindValue(":en", queries + "%");
    queryDb(this, query, tmpData);

    query.prepare("SELECT * FROM dictionary WHERE [bn] LIKE (:bn) OR [en] LIKE (:en) LIMIT 10");
    query.bindValue(":bn", "%" + queries);
    query.bindValue(":en", "%" + queries);
    queryDb(this, query, tmpData);

    if (queries.size() > 2) {
        query.prepare("SELECT * FROM dictionary WHERE [bn] LIKE (:bn) OR [en] LIKE (:en) LIMIT 10");
        query.bindValue(":bn", "%" + queries + "%");
        query.bindValue(":en", "%" + queries + "%");
        queryDb(this, query, tmpData);
    }

    for (auto const& tmp : tmpData)
    {
        bool found = false;
        for (auto const& v : mData)
            if (v->equals(tmp))
                found = true;
        if (!found)
            mData.append(tmp);
    }

    beginInsertRows(QModelIndex(), 0, mData.size() - 1);
    endInsertRows();
}
