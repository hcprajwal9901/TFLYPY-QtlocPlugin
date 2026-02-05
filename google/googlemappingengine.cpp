#include "googlemappingengine.h"
#include "googletilefetcher.h"
#include <QtLocation/private/qgeocameracapabilities_p.h>
#include <QtLocation/private/qgeomaptype_p.h>
#include <QtLocation/private/qgeotiledmap_p.h>
#include <QDebug>

GoogleMappingEngine::GoogleMappingEngine(const QVariantMap &parameters,
                                         QGeoServiceProvider::Error *error,
                                         QString *errorString)
    : QGeoTiledMappingManagerEngine()
{
    Q_UNUSED(parameters)
    
    qDebug() << "GoogleMappingEngine: Constructor called";
    
    // Set capabilities
    QGeoCameraCapabilities capabilities;
    capabilities.setMinimumZoomLevel(1.0);
    capabilities.setMaximumZoomLevel(20.0);
    capabilities.setSupportsBearing(true);
    capabilities.setSupportsTilting(true);
    capabilities.setMinimumTilt(0.0);
    capabilities.setMaximumTilt(60.0);
    capabilities.setMinimumFieldOfView(20.0);
    capabilities.setMaximumFieldOfView(120.0);
    capabilities.setOverzoomEnabled(true);
    setCameraCapabilities(capabilities);
    
    // Set tile size
    setTileSize(QSize(256, 256));
    
    // Create map types
    QList<QGeoMapType> mapTypes;
    
    // 1. Google Street Map
    mapTypes.append(QGeoMapType(QGeoMapType::StreetMap, 
                                 QStringLiteral("Google Street"), 
                                 QStringLiteral("Google Street Map"), 
                                 false, false, 1, 
                                 QByteArrayLiteral("google"), 
                                 capabilities));
    
    // 2. Google Satellite
    mapTypes.append(QGeoMapType(QGeoMapType::SatelliteMapDay, 
                                 QStringLiteral("Google Satellite"), 
                                 QStringLiteral("Google Satellite Map"), 
                                 false, false, 2, 
                                 QByteArrayLiteral("google"), 
                                 capabilities));
    
    // 3. Google Terrain
    mapTypes.append(QGeoMapType(QGeoMapType::TerrainMap, 
                                 QStringLiteral("Google Terrain"), 
                                 QStringLiteral("Google Terrain Map"), 
                                 false, false, 3, 
                                 QByteArrayLiteral("google"), 
                                 capabilities));
    
    // 4. Google Hybrid
    mapTypes.append(QGeoMapType(QGeoMapType::HybridMap, 
                                 QStringLiteral("Google Hybrid"), 
                                 QStringLiteral("Google Hybrid Map"), 
                                 false, false, 4, 
                                 QByteArrayLiteral("google"), 
                                 capabilities));
    
    setSupportedMapTypes(mapTypes);
    qDebug() << "GoogleMappingEngine: Created" << mapTypes.size() << "map types";
    
    // Create tile fetcher
    GoogleTileFetcher *tileFetcher = new GoogleTileFetcher(this);
    setTileFetcher(tileFetcher);
    qDebug() << "GoogleMappingEngine: Tile fetcher created";
    
    *error = QGeoServiceProvider::NoError;
    errorString->clear();
}

GoogleMappingEngine::~GoogleMappingEngine()
{
}

QGeoMap *GoogleMappingEngine::createMap()
{
    return new QGeoTiledMap(this, nullptr);
}
