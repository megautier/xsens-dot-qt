import QtQuick 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Scroll")

    Label {
        id: infoNbSensorLabel
        height: 32
        text: "Nombre de capteurs détectés :"
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 7
    }

    Text {
        id: nbSensor
        anchors.left: infoNbSensorLabel.right
        anchors.leftMargin: 5
        anchors.top: parent.top
        width: 54
        height: 32
        text: scanner.devicesList.length
        font.bold: true
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        textFormat: Text.AutoText
        wrapMode: Text.WrapAtWordBoundaryOrAnywhere
        font.pixelSize: 25
    }


    Button {
        id: searchSensorsButton
        anchors.top: infoNbSensorLabel.bottom
        width: parent.width
        height: 32
        text: "Search xsens dot devices"
        checkable: true
        onClicked: {
            scanner.startScan();
            if (scanner.state) {
                info.dialogText = "Searching...";
                info.visible = true;
            }
        }
    }

    Text {
        id: type
        width: 181
        height: 32
        text: qsTr("Nom")
        horizontalAlignment: Text.AlignHCenter
        anchors.top: searchSensorsButton.bottom
        anchors.topMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        font.pixelSize: 25
    }

    Text {
        id: loadingState
        width: 166
        height: 32
        text: qsTr("Etat de charge")
        font.pixelSize: 25
        anchors.horizontalCenter: parent.horizontalCenter
//        anchors.leftMargin: 15
        anchors.top: searchSensorsButton.bottom
    }

    Dialog {
        id: info
        anchors.centerIn: parent
        visible: false
    }

    ScrollView {
        width: parent.width
        clip: true
        anchors.top: type.bottom
        anchors.bottom: parent.bottom

        ListView {
            width: parent.width
            model: scanner.devicesList

            delegate: Rectangle {
                id: box
                height:100
                width: parent.width
                color: "lightsteelblue"
                border.width: 2
                border.color: "black"
                radius: 5

                Component.onCompleted: {
                    info.visible = false;
                }
                Text {
                    id: sensorName
                    text: modelData.deviceName + "\n" + modelData.macAddress
                    anchors.top: parent.top
                    anchors.topMargin: 5
                    anchors.left: parent.left
                    anchors.leftMargin: 5
                    font.pixelSize: 20
                }
                Text {
                    id: batteryState
                    anchors.top: parent.top
                    anchors.topMargin: 5
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.pixelSize: 20
                    text: modelData.battery
                }
                Text {
                    id: chargingState
                    anchors.top: parent.top
                    anchors.topMargin: 5
                    anchors.right: parent.right
                    anchors.rightMargin: 5
                    font.pixelSize: 20
                    text: (modelData.charging)?"charge":"décharge"
                }
            }

        }
    }
}
