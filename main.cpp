#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDirIterator>
#include "wordslistmodel.h"


int main(int argc, char *argv[])
{
    for (auto v : QList({":", "qrc:", "assets:"}))
    {
        QDirIterator it(v, QDirIterator::Subdirectories);
        while (it.hasNext())
            qDebug() << it.next();
    }

    QGuiApplication app(argc, argv);

    WordsListModel wordsListModel;

    QQmlApplicationEngine engine;
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.rootContext()->setContextProperty("wordsListModel", &wordsListModel);
    engine.loadFromModule("Dictionary", "Main");

    return app.exec();
}
