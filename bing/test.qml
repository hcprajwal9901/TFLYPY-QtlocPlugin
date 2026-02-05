import QtQuick 2.15
import QtQuick.Window 2.15
import QtLocation 5.15
import QtPositioning 5.15

Window {
    width: 1024
    height: 768
    visible: true
    title: qsTr("Bing Maps QtLocation Test")

    Plugin {
        id: bingPlugin
        name: "bing"
    }

    Map {
        id: map
        anchors.fill: parent
        plugin: bingPlugin
        center: QtPositioning.coordinate(37.7749, -122.4194) // San Francisco
        zoomLevel: 10

        // Toggle between Satellite and Hybrid
        activeMapType: map.supportedMapTypes[1] // Default to Hybrid

        Component.onCompleted: {
            console.log("Supported Map Types:")
            for (var i = 0; i < supportedMapTypes.length; i++) {
                console.log(i + ": " + supportedMapTypes[i].name)
            }
        }
    }

    Rectangle {
        anchors.top: parent.top
        anchors.right: parent.right
        width: 200
        height: 100
        color: "white"
        opacity: 0.8
        Column {
            anchors.centerIn: parent
            spacing: 10
            Text { text: "Map Type:" }
            Row {
                spacing: 10
                Button {
                    text: "Satellite"
                    onClicked: map.activeMapType = map.supportedMapTypes[0]
                }
                Button {
                    text: "Hybrid"
                    onClicked: map.activeMapType = map.supportedMapTypes[1]
                }
            }
        }
    }
}
