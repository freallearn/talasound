import QtQuick 2.4
import QtQuick.Templates 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2

Item {
    width: 400
    height: 400

    Row {
        id: row
        x: 0
        y: 0
        width: 200
        height: 400

        ListView {
            id: listView
            width: 121
            height: 164
            model: ListModel {
                ListElement {
                    name: "Grey"
                    colorCode: "grey"
                }

                ListElement {
                    name: "Red"
                    colorCode: "red"
                }

                ListElement {
                    name: "Blue"
                    colorCode: "blue"
                }

                ListElement {
                    name: "Green"
                    colorCode: "green"
                }
            }
            delegate: Item {
                x: 5
                width: 80
                height: 40
                Row {
                    id: row1
                    spacing: 10
                    Rectangle {
                        width: 40
                        height: 40
                        color: colorCode
                    }

                    Text {
                        text: name
                        anchors.verticalCenter: parent.verticalCenter
                        font.bold: true
                    }
                }
            }
        }
    }

    BusyIndicator {
        id: busyIndicator
        x: 49
        y: 221
    }

    Switch {
        id: switch1
        x: 120
        y: 268
        text: qsTr("Switch")
    }
}
