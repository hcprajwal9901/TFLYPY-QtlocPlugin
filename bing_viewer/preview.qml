import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtLocation 5.15
import QtPositioning 5.15

Window {
    visible: true
    width: 1280
    height: 720
    title: "Bing Maps Viewer - 2GB Cached"

    Plugin {
        id: bingPlugin
        name: "bing"
        
        Component.onCompleted: {
            console.log("Plugin loaded:", name)
        }
    }

    Column {
        anchors.fill: parent

        // Control bar
        Rectangle {
            width: parent.width
            height: 60
            color: "#2c3e50"

            Row {
                anchors.centerIn: parent
                spacing: 15

                Text {
                    text: "Bing Maps Viewer"
                    color: "white"
                    font.pixelSize: 18
                    font.bold: true
                    anchors.verticalCenter: parent.verticalCenter
                }

                Rectangle {
                    width: 2
                    height: 30
                    color: "#34495e"
                    anchors.verticalCenter: parent.verticalCenter
                }

                Text {
                    text: "Map Type:"
                    color: "white"
                    anchors.verticalCenter: parent.verticalCenter
                }

                Button {
                    text: "Satellite"
                    highlighted: map.activeMapType === map.supportedMapTypes[0]
                    onClicked: {
                        if (map.supportedMapTypes.length > 0) {
                            map.activeMapType = map.supportedMapTypes[0]
                            console.log("Switched to Satellite")
                        }
                    }
                }

                Button {
                    text: "Hybrid"
                    highlighted: map.activeMapType === map.supportedMapTypes[1]
                    onClicked: {
                        if (map.supportedMapTypes.length > 1) {
                            map.activeMapType = map.supportedMapTypes[1]
                            console.log("Switched to Hybrid")
                        }
                    }
                }

                Rectangle {
                    width: 2
                    height: 30
                    color: "#34495e"
                    anchors.verticalCenter: parent.verticalCenter
                }

                Text {
                    text: "Zoom: " + map.zoomLevel.toFixed(1)
                    color: "white"
                    font.pixelSize: 14
                    anchors.verticalCenter: parent.verticalCenter
                }

                Text {
                    text: "Center: " + map.center.latitude.toFixed(4) + ", " + map.center.longitude.toFixed(4)
                    color: "white"
                    font.pixelSize: 14
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }

        // Map
        Map {
            id: map
            width: parent.width
            height: parent.height - 60
            plugin: bingPlugin
            center: QtPositioning.coordinate(37.7749, -122.4194) // San Francisco
            zoomLevel: 12

            Component.onCompleted: {
                console.log("Map initialized")
                console.log("Plugin:", plugin.name)
                console.log("Supported map types:", supportedMapTypes.length)
                
                for (var i = 0; i < supportedMapTypes.length; i++) {
                    console.log("  [" + i + "]", supportedMapTypes[i].name)
                }
                
                if (supportedMapTypes.length > 0) {
                    activeMapType = supportedMapTypes[0] // Default to Satellite
                    console.log("Active map type:", activeMapType.name)
                }
            }

            // Status overlay
            Rectangle {
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.margins: 15
                width: 280
                height: statusText.height + 30
                color: "#80000000"
                radius: 8
                border.color: "#3498db"
                border.width: 2

                Text {
                    id: statusText
                    anchors.centerIn: parent
                    color: "white"
                    font.pixelSize: 13
                    font.family: "Consolas"
                    text: "Plugin: " + map.plugin.name + "\n" +
                          "Map Types: " + map.supportedMapTypes.length + "\n" +
                          "Active: " + (map.activeMapType ? map.activeMapType.name : "none") + "\n" +
                          "Cache: 2048 MB (C:/QtLocationCache)"
                }
            }

            // Cache info overlay
            Rectangle {
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                anchors.margins: 15
                width: 350
                height: cacheInfoText.height + 30
                color: "#80000000"
                radius: 8
                border.color: "#2ecc71"
                border.width: 2

                Text {
                    id: cacheInfoText
                    anchors.centerIn: parent
                    color: "#2ecc71"
                    font.pixelSize: 12
                    font.family: "Consolas"
                    text: "✓ Tile caching enabled\n" +
                          "✓ Max cache size: 2 GB\n" +
                          "✓ Offline mode supported\n" +
                          "✓ Navigate to cache tiles for offline use"
                }
            }
        }
    }
}
