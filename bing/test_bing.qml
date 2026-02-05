import QtQuick 2.15
import QtQuick.Window 2.15
import QtLocation 5.15
import QtPositioning 5.15

Window {
    visible: true
    width: 1024
    height: 768
    title: "Bing Maps Test - Linux"

    Plugin {
        id: bingPlugin
        name: "bing"
        
        Component.onCompleted: {
            console.log("Plugin name:", name)
            console.log("Plugin loaded")
        }
    }

    Map {
        id: map
        anchors.fill: parent
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
                activeMapType = supportedMapTypes[0]
                console.log("Active map type:", activeMapType.name)
            } else {
                console.error("ERROR: No map types available!")
            }
        }
        
        // Status overlay
        Rectangle {
            anchors.centerIn: parent
            width: 400
            height: 150
            color: "#80000000"
            radius: 10
            border.color: supportedMapTypes.length > 0 ? "#00ff00" : "#ff0000"
            border.width: 3

            Column {
                anchors.centerIn: parent
                spacing: 10

                Text {
                    text: "Bing Maps Plugin Test"
                    color: "white"
                    font.pixelSize: 20
                    font.bold: true
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                Text {
                    text: "Plugin: " + map.plugin.name
                    color: "white"
                    font.pixelSize: 16
                }

                Text {
                    text: "Map Types: " + map.supportedMapTypes.length
                    color: map.supportedMapTypes.length > 0 ? "#00ff00" : "#ff0000"
                    font.pixelSize: 16
                    font.bold: true
                }

                Text {
                    text: "Active: " + (map.activeMapType ? map.activeMapType.name : "none")
                    color: "white"
                    font.pixelSize: 14
                }

                Text {
                    text: map.supportedMapTypes.length > 0 ? "✓ Plugin Working" : "✗ Plugin Failed"
                    color: map.supportedMapTypes.length > 0 ? "#00ff00" : "#ff0000"
                    font.pixelSize: 18
                    font.bold: true
                    anchors.horizontalCenter: parent.horizontalCenter
                }
            }
        }
    }
}
