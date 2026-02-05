#pragma once

#include <QtLocation/private/qgeotilefetcher_p.h>

class QGeoTiledMappingManagerEngine;
class QNetworkAccessManager;

class BingTileFetcher : public QGeoTileFetcher
{
public:
    explicit BingTileFetcher(QGeoTiledMappingManagerEngine *engine);
    ~BingTileFetcher() override;

    QGeoTiledMapReply *getTileImage(const QGeoTileSpec &spec) override;

private:
    QGeoTiledMappingManagerEngine *m_engine;
    QNetworkAccessManager *m_networkManager;
};
