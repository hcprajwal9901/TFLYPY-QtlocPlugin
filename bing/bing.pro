QT += location location-private positioning network

TARGET = qtgeoservices_bing
PLUGIN_TYPE = geoservices
PLUGIN_CLASS_NAME = QGeoServiceProviderFactoryBing
TEMPLATE = lib
CONFIG += plugin

DESTDIR = $$[QT_INSTALL_PLUGINS]/geoservices

HEADERS += \
    bingplugin.h \
    bingmappingengine.h \
    bingtilefetcher.h \
    bingtiledmapreply.h

SOURCES += \
    bingplugin.cpp \
    bingmappingengine.cpp \
    bingtilefetcher.cpp \
    bingtiledmapreply.cpp

OTHER_FILES += \
    bing.json \
    qmldir

# Ensure the qmldir is treated as a resource or installed correctly
# For development, we just need the plugin to be in the right place.
