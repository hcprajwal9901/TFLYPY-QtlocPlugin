#ifndef GOOGLEMAPPINGENGINE_H
#define GOOGLEMAPPINGENGINE_H

#include <QtLocation/private/qgeotiledmappingmanagerengine_p.h>
#include <QGeoServiceProvider>

class GoogleTileFetcher;

class GoogleMappingEngine : public QGeoTiledMappingManagerEngine
{
    Q_OBJECT

public:
    GoogleMappingEngine(const QVariantMap &parameters,
                        QGeoServiceProvider::Error *error,
                        QString *errorString);
    ~GoogleMappingEngine();

protected:
    QGeoMap *createMap() override;
};

#endif // GOOGLEMAPPINGENGINE_H
