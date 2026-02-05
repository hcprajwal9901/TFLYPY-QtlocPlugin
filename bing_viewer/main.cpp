#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    
    qDebug() << "Bing Maps Viewer starting...";
    
    // Load QML
    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/preview.qml"));
    
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    
    engine.load(url);
    
    if (engine.rootObjects().isEmpty()) {
        qCritical() << "Failed to load QML file:" << url;
        return -1;
    }
    
    qDebug() << "Application started successfully";
    return app.exec();
}
