import QtQuick 2.15
import QtQuick.Window 2.15
import QtLocation 5.15
import QtPositioning 5.15

Window {
    visible: true
    width: 1024
    height: 768
    title: "Google Maps Test"

    Plugin {
        id: googlePlugin
        name: "google"
        
        Component.onCompleted: {
            console.log("=== PLUGIN TEST ===")
            console.log("Plugin:", name)
        }
    }

    Map {
        id: map
        anchors.fill: parent
        plugin: googlePlugin
        center: QtPositioning.coordinate(37.7749, -122.4194) // San Francisco
        zoomLevel: 12
        
        Component.onCompleted: {
            console.log("Map types:", supportedMapTypes.length)
            for (var i = 0; i < supportedMapTypes.length; i++) {
                console.log("  [" + i + "]", supportedMapTypes[i].name)
            }
            console.log("Active map type:", activeMapType.name)
        }
        
        // Status overlay
        Rectangle {
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.margins: 20
            width: statusText.width + 40
            height: statusText.height + 20
            color: "#CC000000"
            radius: 10
            
            Text {
                id: statusText
                anchors.centerIn: parent
                color: "white"
                font.pixelSize: 16
                font.bold: true
                text: "Google Maps Plugin Test\n" +
                      "Plugin: " + googlePlugin.name + "\n" +
                      "Map Types: " + map.supportedMapTypes.length + "\n" +
                      "Active: " + map.activeMapType.name + "\n" +
                      (map.supportedMapTypes.length > 0 ? "✓ Plugin Working" : "✗ Plugin Failed")
            }
        }
        
        // Map type switcher
        Row {
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.margins: 20
            spacing: 10
            
            Repeater {
                model: map.supportedMapTypes
                
                Rectangle {
                    width: 100
                    height: 40
                    color: map.activeMapType === modelData ? "#4CAF50" : "#2196F3"
                    radius: 5
                    
                    Text {
                        anchors.centerIn: parent
                        color: "white"
                        font.pixelSize: 12
                        text: modelData.name
                    }
                    
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            map.activeMapType = modelData
                            console.log("Switched to:", modelData.name)
                        }
                    }
                }
            }
        }
    }
}
