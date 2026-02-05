#include "googletiledmapreply.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QDebug>

GoogleTiledMapReply::GoogleTiledMapReply(const QGeoTileSpec &spec,
                                         QNetworkAccessManager *networkManager,
                                         const QString &url,
                                         QObject *parent)
    : QGeoTiledMapReply(spec, parent)
    , m_reply(nullptr)
{
    QNetworkRequest request(url);
    request.setRawHeader("User-Agent", "Mozilla/5.0");
    request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::PreferCache);
    
    m_reply = networkManager->get(request);
    
    connect(m_reply, &QNetworkReply::finished, this, &GoogleTiledMapReply::onNetworkReplyFinished);
    connect(m_reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::errorOccurred),
            this, &GoogleTiledMapReply::onNetworkReplyError);
    
    qDebug() << "GoogleTiledMapReply: Network request started for:" << url;
}

GoogleTiledMapReply::~GoogleTiledMapReply()
{
    if (m_reply) {
        m_reply->deleteLater();
    }
}

void GoogleTiledMapReply::onNetworkReplyFinished()
{
    if (!m_reply) {
        return;
    }
    
    if (m_reply->error() == QNetworkReply::NoError) {
        QByteArray data = m_reply->readAll();
        qDebug() << "GoogleTiledMapReply: SUCCESS - Received" << data.size() << "bytes";
        
        setMapImageData(data);
        setMapImageFormat("png");
        setCached(true);
        setFinished(true);
    } else {
        qDebug() << "GoogleTiledMapReply: ERROR -" << m_reply->error() << m_reply->errorString();
        qDebug() << "GoogleTiledMapReply: URL was:" << m_reply->url().toString();
        setError(QGeoTiledMapReply::CommunicationError, m_reply->errorString());
        setFinished(true);
    }
}

void GoogleTiledMapReply::onNetworkReplyError(QNetworkReply::NetworkError error)
{
    if (!m_reply) {
        return;
    }
    
    qDebug() << "GoogleTiledMapReply: Network error occurred:" << error;
    qDebug() << "GoogleTiledMapReply: Error string:" << m_reply->errorString();
}
