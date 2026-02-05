#pragma once

#include <QObject>
#include <QtLocation/QGeoServiceProvider>
#include <QtLocation/qgeoserviceproviderfactory.h>

class QGeoMappingManagerEngine;

class QGeoServiceProviderFactoryBing : public QObject, public QGeoServiceProviderFactory
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.qt.geoservice.serviceproviderfactory/5.0" FILE "bing.json")
    Q_INTERFACES(QGeoServiceProviderFactory)

public:
    QGeoMappingManagerEngine *createMappingManagerEngine(
        const QVariantMap &parameters,
        QGeoServiceProvider::Error *error,
        QString *errorString
    ) const override;
};
