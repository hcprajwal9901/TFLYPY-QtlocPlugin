#include "googleplugin.h"
#include "googlemappingengine.h"
#include <QDebug>

QGeoMappingManagerEngine *QGeoServiceProviderFactoryGoogle::createMappingManagerEngine(
    const QVariantMap &parameters,
    QGeoServiceProvider::Error *error,
    QString *errorString) const
{
    qDebug() << "GooglePlugin: createMappingManagerEngine called!";
    
    Q_UNUSED(parameters)
    
    GoogleMappingEngine *engine = new GoogleMappingEngine(parameters, error, errorString);
    
    if (!engine) {
        *error = QGeoServiceProvider::NotSupportedError;
        *errorString = "Failed to create Google mapping engine";
        qDebug() << "GooglePlugin: ERROR - Failed to create engine";
        return nullptr;
    }
    
    qDebug() << "GooglePlugin: Engine created, map types:" << engine->supportedMapTypes().size();
    return engine;
}
