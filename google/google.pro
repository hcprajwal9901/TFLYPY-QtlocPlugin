QT += location location-private positioning network

TARGET = qtgeoservices_google
PLUGIN_TYPE = geoservices
PLUGIN_CLASS_NAME = QGeoServiceProviderFactoryGoogle
TEMPLATE = lib
CONFIG += plugin

DESTDIR = $$[QT_INSTALL_PLUGINS]/geoservices

HEADERS += \
    googleplugin.h \
    googlemappingengine.h \
    googletilefetcher.h \
    googletiledmapreply.h

SOURCES += \
    googleplugin.cpp \
    googlemappingengine.cpp \
    googletilefetcher.cpp \
    googletiledmapreply.cpp

OTHER_FILES += \
    google.json

# Linux-specific: Remove location-private and add manual include paths
unix:!macx {
    QT -= location-private
    INCLUDEPATH += /usr/include/x86_64-linux-gnu/qt5/QtLocation/private
    INCLUDEPATH += /usr/include/x86_64-linux-gnu/qt5/QtPositioning/private
}

