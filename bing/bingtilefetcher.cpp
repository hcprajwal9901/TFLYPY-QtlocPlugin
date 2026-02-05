#include "bingtilefetcher.h"
#include "bingtiledmapreply.h"

#include <QtLocation/private/qgeotilespec_p.h>
#include <QtLocation/private/qgeotiledmappingmanagerengine_p.h>
#include <QNetworkAccessManager>
#include <QDebug>

static QString tileXYToQuadKey(int x, int y, int z)
{
    QString quadKey;
    for (int i = z; i > 0; --i) {
        char digit = '0';
        int mask = 1 << (i - 1);
        if (x & mask) digit++;
        if (y & mask) digit += 2;
        quadKey.append(digit);
    }
    return quadKey;
}

BingTileFetcher::BingTileFetcher(QGeoTiledMappingManagerEngine *engine)
    : QGeoTileFetcher(engine)
    , m_engine(engine)
    , m_networkManager(new QNetworkAccessManager(this))
{
    qDebug() << "BingTileFetcher: Created with network manager";
}

BingTileFetcher::~BingTileFetcher() = default;

QGeoTiledMapReply *BingTileFetcher::getTileImage(const QGeoTileSpec &spec)
{
    const QString quadKey = tileXYToQuadKey(spec.x(), spec.y(), spec.zoom());
    const int sub = (spec.x() + spec.y()) % 4;
    const char type = (spec.mapId() == 2) ? 'h' : 'a';

    const QString url =
        QString("http://ecn.t%1.tiles.virtualearth.net/tiles/%2%3.jpg?g=2981&mkt=en-US")
            .arg(sub)
            .arg(type)
            .arg(quadKey);

    qDebug() << "BingTileFetcher: Requesting tile - zoom:" << spec.zoom() 
             << "x:" << spec.x() << "y:" << spec.y() 
             << "mapId:" << spec.mapId() << "quadkey:" << quadKey;
    qDebug() << "BingTileFetcher: URL:" << url;

    return new BingTiledMapReply(spec, m_networkManager, url, this);
}
