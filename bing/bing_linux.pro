QT += location positioning network

# Add private headers manually for Linux
INCLUDEPATH += /usr/include/x86_64-linux-gnu/qt5/QtLocation/5.15.13 \
               /usr/include/x86_64-linux-gnu/qt5/QtLocation/5.15.13/QtLocation

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
    bing.json

# Ensure the qmldir is treated as a resource or installed correctly
# For development, we just need the plugin to be in the right place.
