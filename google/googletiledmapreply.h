#ifndef GOOGLETILEDMAPREPLY_H
#define GOOGLETILEDMAPREPLY_H

#include <QtLocation/private/qgeotiledmapreply_p.h>
#include <QNetworkReply>

class QNetworkAccessManager;

class GoogleTiledMapReply : public QGeoTiledMapReply
{
    Q_OBJECT

public:
    GoogleTiledMapReply(const QGeoTileSpec &spec,
                        QNetworkAccessManager *networkManager,
                        const QString &url,
                        QObject *parent = nullptr);
    ~GoogleTiledMapReply();

private slots:
    void onNetworkReplyFinished();
    void onNetworkReplyError(QNetworkReply::NetworkError error);

private:
    QNetworkReply *m_reply;
};

#endif // GOOGLETILEDMAPREPLY_H
