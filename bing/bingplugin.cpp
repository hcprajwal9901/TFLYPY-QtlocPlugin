#include "bingplugin.h"
#include "bingmappingengine.h"
#include <QDebug>

QGeoMappingManagerEngine *
QGeoServiceProviderFactoryBing::createMappingManagerEngine(
    const QVariantMap &parameters,
    QGeoServiceProvider::Error *error,
    QString *errorString) const
{
    qDebug() << "BingPlugin: createMappingManagerEngine called!";
    auto *engine = new BingMappingEngine(parameters, error, errorString);
    qDebug() << "BingPlugin: Engine created, map types:" << engine->supportedMapTypes().count();
    return engine;
}
