#include <QApplication>
#include <QQmlContext>
#include <QQmlApplicationEngine>
#include "transcoding.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Transcoding *transcoding = new Transcoding(&app);

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("transcoding", transcoding);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}

