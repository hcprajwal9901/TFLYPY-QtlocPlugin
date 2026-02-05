#pragma once

#include <QtLocation/QGeoServiceProvider>
#include <QtLocation/private/qgeotiledmappingmanagerengine_p.h>

class BingMappingEngine : public QGeoTiledMappingManagerEngine
{
    Q_OBJECT
public:
    BingMappingEngine(
        const QVariantMap &parameters,
        QGeoServiceProvider::Error *error,
        QString *errorString
    );

protected:
    QGeoMap *createMap() override;
};
