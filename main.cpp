#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "wordslistmodel.h"


int main(int argc, char *argv[])
{
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
