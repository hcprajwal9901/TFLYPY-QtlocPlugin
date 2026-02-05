QT += quick location positioning

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp

RESOURCES += qml.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

TARGET = bing_viewer

# Copy Bing plugin to build directory
win32 {
    BING_PLUGIN_SOURCE = $$shell_quote($$shell_path(C:/Qt/5.15.2/msvc2019_64/plugins/geoservices/qtgeoservices_bing.dll))
    
    CONFIG(debug, debug|release) {
        BING_PLUGIN_DEST = $$shell_quote($$shell_path($$OUT_PWD/debug/plugins/geoservices/))
    } else {
        BING_PLUGIN_DEST = $$shell_quote($$shell_path($$OUT_PWD/release/plugins/geoservices/))
    }
    
    # Create plugins/geoservices directory and copy plugin
    QMAKE_POST_LINK += $$quote(if not exist $$BING_PLUGIN_DEST mkdir $$BING_PLUGIN_DEST $$escape_expand(\n\t))
    QMAKE_POST_LINK += $$quote(copy /Y $$BING_PLUGIN_SOURCE $$BING_PLUGIN_DEST $$escape_expand(\n\t))
}
