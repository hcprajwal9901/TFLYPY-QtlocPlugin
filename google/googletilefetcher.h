#ifndef GOOGLETILEFETCHER_H
#define GOOGLETILEFETCHER_H

#include <QtLocation/private/qgeotilefetcher_p.h>

class QNetworkAccessManager;

class GoogleTileFetcher : public QGeoTileFetcher
{
    Q_OBJECT

public:
    explicit GoogleTileFetcher(QGeoTiledMappingManagerEngine *engine);
    ~GoogleTileFetcher();

private:
    QGeoTiledMapReply *getTileImage(const QGeoTileSpec &spec) override;
    
    QString getGoogleUrl(int x, int y, int zoom, int mapId) const;
    QString getSecurityWord(int x, int y) const;
    
    QNetworkAccessManager *m_networkManager;
};

#endif // GOOGLETILEFETCHER_H
