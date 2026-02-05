#include "bingtiledmapreply.h"

#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QDebug>

BingTiledMapReply::BingTiledMapReply(
    const QGeoTileSpec &spec,
    QNetworkAccessManager *nam,
    const QString &url,
    QObject *parent
)
    : QGeoTiledMapReply(spec, parent)
    , m_reply(nullptr)
{
    if (!nam) {
        qDebug() << "BingTiledMapReply: ERROR - No network access manager!";
        setError(QGeoTiledMapReply::CommunicationError,
                 QStringLiteral("No network access manager"));
        setFinished(true);
        return;
    }

    QNetworkRequest req(url);
    req.setAttribute(QNetworkRequest::CacheLoadControlAttribute,
                     QNetworkRequest::PreferCache);

    m_reply = nam->get(req);
    qDebug() << "BingTiledMapReply: Network request started for:" << url;

    connect(m_reply, &QNetworkReply::finished,
            this, &BingTiledMapReply::replyFinished);
    connect(m_reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error),
            this, &BingTiledMapReply::replyError);
}

void BingTiledMapReply::abort()
{
    if (m_reply)
        m_reply->abort();
}

void BingTiledMapReply::replyFinished()
{
    if (!m_reply)
        return;

    if (m_reply->error() == QNetworkReply::NoError) {
        QByteArray data = m_reply->readAll();
        qDebug() << "BingTiledMapReply: SUCCESS - Received" << data.size() << "bytes";
        setMapImageData(data);
        setMapImageFormat("jpeg");
        setCached(true);
    } else {
        qDebug() << "BingTiledMapReply: ERROR -" << m_reply->error() << m_reply->errorString();
        qDebug() << "BingTiledMapReply: URL was:" << m_reply->url().toString();
        setError(QGeoTiledMapReply::CommunicationError,
                 m_reply->errorString());
    }

    setFinished(true);
    m_reply->deleteLater();
    m_reply = nullptr;
}

BingTiledMapReply::~BingTiledMapReply()
{
    if (m_reply) {
        m_reply->deleteLater();
        m_reply = nullptr;
    }
}

void BingTiledMapReply::replyError(QNetworkReply::NetworkError error)
{
    qDebug() << "BingTiledMapReply: Network error occurred:" << error;
    if (m_reply) {
        qDebug() << "BingTiledMapReply: Error string:" << m_reply->errorString();
    }
}
