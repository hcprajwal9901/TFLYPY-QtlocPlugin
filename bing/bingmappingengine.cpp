#include "bingmappingengine.h"
#include "bingtilefetcher.h"

#include <QtLocation/private/qgeomaptype_p.h>
#include <QtLocation/private/qgeocameracapabilities_p.h>
#include <QtLocation/private/qgeotiledmap_p.h>
#include <QtLocation/private/qabstractgeotilecache_p.h>
#include <QDebug>

BingMappingEngine::BingMappingEngine(
    const QVariantMap &parameters,
    QGeoServiceProvider::Error *error,
    QString *errorString
)
    : QGeoTiledMappingManagerEngine()
{
    qDebug() << "BingMappingEngine: Constructor called";
    Q_UNUSED(parameters)
    Q_UNUSED(error)
    Q_UNUSED(errorString)
    
    setTileFetcher(new BingTileFetcher(this));
    setTileSize(QSize(256, 256));

    QGeoCameraCapabilities caps;
    caps.setMinimumZoomLevel(1.0);
    caps.setMaximumZoomLevel(19.0);
    setCameraCapabilities(caps);

    QList<QGeoMapType> mapTypes;
    mapTypes.append(QGeoMapType(QGeoMapType::SatelliteMapDay, 
                                 QStringLiteral("Bing Satellite"), 
                                 QStringLiteral("Bing Satellite"), 
                                 false, false, 1, 
                                 QByteArrayLiteral("bing"), 
                                 caps));
    mapTypes.append(QGeoMapType(QGeoMapType::HybridMap, 
                                 QStringLiteral("Bing Hybrid"), 
                                 QStringLiteral("Bing Hybrid"), 
                                 false, false, 2, 
                                 QByteArrayLiteral("bing"), 
                                 caps));

    qDebug() << "BingMappingEngine: Created" << mapTypes.count() << "map types";
    setSupportedMapTypes(mapTypes);
    qDebug() << "BingMappingEngine: supportedMapTypes() returns" << supportedMapTypes().count();
}

QGeoMap *BingMappingEngine::createMap()
{
    return new QGeoTiledMap(this, 0);
}
