#ifndef GOOGLEPLUGIN_H
#define GOOGLEPLUGIN_H

#include <QObject>
#include <QGeoServiceProviderFactory>

class QGeoServiceProviderFactoryGoogle : public QObject, public QGeoServiceProviderFactory
{
    Q_OBJECT
    Q_INTERFACES(QGeoServiceProviderFactory)
    Q_PLUGIN_METADATA(IID "org.qt-project.qt.geoservice.serviceproviderfactory/5.0" FILE "google.json")

public:
    QGeoMappingManagerEngine *createMappingManagerEngine(
        const QVariantMap &parameters,
        QGeoServiceProvider::Error *error,
        QString *errorString) const override;
};

#endif // GOOGLEPLUGIN_H
