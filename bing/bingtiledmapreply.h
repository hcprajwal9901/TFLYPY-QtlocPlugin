#ifndef BINGTILEDMAPREPLY_H
#define BINGTILEDMAPREPLY_H

#include <QtLocation/private/qgeotiledmapreply_p.h>
#include <QtNetwork/QNetworkReply>

class BingTiledMapReply : public QGeoTiledMapReply
{
    Q_OBJECT
public:
    BingTiledMapReply(
        const QGeoTileSpec &spec,
        QNetworkAccessManager *nam,
        const QString &url,
        QObject *parent = nullptr
    );

    ~BingTiledMapReply() override;

    void abort() override;

private slots:
    void replyFinished();
    void replyError(QNetworkReply::NetworkError error);

private:
    QNetworkReply *m_reply;
};

#endif
