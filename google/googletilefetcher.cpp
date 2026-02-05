#include "googletilefetcher.h"
#include "googletiledmapreply.h"
#include <QNetworkAccessManager>
#include <QDebug>

GoogleTileFetcher::GoogleTileFetcher(QGeoTiledMappingManagerEngine *engine)
    : QGeoTileFetcher(engine)
    , m_networkManager(new QNetworkAccessManager(this))
{
    qDebug() << "GoogleTileFetcher: Created with network manager";
}

GoogleTileFetcher::~GoogleTileFetcher()
{
}

QGeoTiledMapReply *GoogleTileFetcher::getTileImage(const QGeoTileSpec &spec)
{
    int x = spec.x();
    int y = spec.y();
    int zoom = spec.zoom();
    int mapId = spec.mapId();
    
    QString url = getGoogleUrl(x, y, zoom, mapId);
    
    qDebug() << "GoogleTileFetcher: Requesting tile - zoom:" << zoom 
             << "x:" << x << "y:" << y << "mapId:" << mapId;
    qDebug() << "GoogleTileFetcher: URL:" << url;
    
    return new GoogleTiledMapReply(spec, m_networkManager, url, this);
}

QString GoogleTileFetcher::getGoogleUrl(int x, int y, int zoom, int mapId) const
{
    // Server sharding: (x + y) % 4
    int server = (x + y) % 4;
    
    // Map type lyrs parameter
    QString lyrs;
    switch (mapId) {
        case 1: lyrs = "m"; break;      // Street
        case 2: lyrs = "s"; break;      // Satellite
        case 3: lyrs = "t,r"; break;    // Terrain
        case 4: lyrs = "y"; break;      // Hybrid
        default: lyrs = "m"; break;
    }
    
    // Security words
    QString sec1 = getSecurityWord(x, y);
    QString sec2 = getSecurityWord(x, y);
    
    // Google Maps tile URL format
    // http://mt{server}.google.com/vt/lyrs={lyrs}&hl={language}&x={x}&s={sec1}&y={y}&z={zoom}&s={sec2}
    QString url = QString("http://mt%1.google.com/vt/lyrs=%2&hl=en&x=%3&s=%4&y=%5&z=%6&s=%7")
                      .arg(server)
                      .arg(lyrs)
                      .arg(x)
                      .arg(sec1)
                      .arg(y)
                      .arg(zoom)
                      .arg(sec2);
    
    return url;
}

QString GoogleTileFetcher::getSecurityWord(int x, int y) const
{
    // QGroundControl-style security word generation
    // Based on "Galileo" string manipulation
    const QString baseWord = "Galileo";
    int index = (x + y) % baseWord.length();
    return QString(baseWord[index]);
}
