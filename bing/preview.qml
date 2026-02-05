import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtLocation 5.15
import QtPositioning 5.15

Window {
    visible: true
    width: 1024
    height: 768
    title: "Bing Maps Plugin Preview"

    Plugin {
        id: bingPlugin
        name: "bing"
        
        Component.onCompleted: {
            console.log("Plugin name:", name)
            console.log("Plugin loaded:", name !== "")
        }
    }

    Column {
        anchors.fill: parent

        // Control bar
        Rectangle {
            width: parent.width
            height: 50
            color: "#2c3e50"

            Row {
                anchors.centerIn: parent
                spacing: 10

                Text {
                    text: "Map Type:"
                    color: "white"
                    anchors.verticalCenter: parent.verticalCenter
                }

                Button {
                    text: "Satellite"
                    onClicked: {
                        if (map.supportedMapTypes.length > 0) {
                            map.activeMapType = map.supportedMapTypes[0]
                            console.log("Switched to Satellite (mapId:", map.activeMapType.mapId, ")")
                        }
                    }
                }

                Button {
                    text: "Hybrid"
                    onClicked: {
                        if (map.supportedMapTypes.length > 1) {
                            map.activeMapType = map.supportedMapTypes[1]
                            console.log("Switched to Hybrid (mapId:", map.activeMapType.mapId, ")")
                        }
                    }
                }

                Text {
                    text: "Zoom: " + map.zoomLevel.toFixed(1)
                    color: "white"
                    anchors.verticalCenter: parent.verticalCenter
                }

                Text {
                    text: "Center: " + map.center.latitude.toFixed(4) + ", " + map.center.longitude.toFixed(4)
                    color: "white"
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }

        // Map
        Map {
            id: map
            width: parent.width
            height: parent.height - 50
            plugin: bingPlugin
            center: QtPositioning.coordinate(37.7749, -122.4194) // San Francisco
            zoomLevel: 12

            Component.onCompleted: {
                console.log("Map plugin:", plugin.name)
                console.log("Supported map types:", supportedMapTypes.length)
                for (var i = 0; i < supportedMapTypes.length; i++) {
                    console.log("  [" + i + "]", 
                                "name:", supportedMapTypes[i].name,
                                "mapId:", supportedMapTypes[i].mapId,
                                "style:", supportedMapTypes[i].style)
                }
                
                if (supportedMapTypes.length > 0) {
                    activeMapType = supportedMapTypes[0]
                    console.log("Active map type:", activeMapType.name, "(mapId:", activeMapType.mapId, ")")
                } else {
                    console.error("ERROR: No map types available! Plugin may not be loaded correctly.")
                }
            }

            // Mouse area for debugging clicks
            MouseArea {
                anchors.fill: parent
                acceptedButtons: Qt.LeftButton | Qt.RightButton
                
                onClicked: {
                    var coord = map.toCoordinate(Qt.point(mouse.x, mouse.y))
                    console.log("Clicked at:", coord.latitude.toFixed(6), coord.longitude.toFixed(6))
                }
            }

            // Debug text overlay
            Rectangle {
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.margins: 10
                width: 250
                height: debugText.height + 20
                color: "#80000000"
                radius: 5

                Text {
                    id: debugText
                    anchors.centerIn: parent
                    color: "white"
                    font.pixelSize: 12
                    text: "Plugin: " + map.plugin.name + "\n" +
                          "Map Types: " + map.supportedMapTypes.length + "\n" +
                          "Active: " + (map.activeMapType ? map.activeMapType.name : "none") + "\n" +
                          "MapId: " + (map.activeMapType ? map.activeMapType.mapId : "N/A")
                }
            }
        }
    }
}
