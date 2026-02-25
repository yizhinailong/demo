import QtQuick

Item {
    id: rootId
    width: notifierRectId.width
    height: notifierRectId.height

    property alias rectColor: notifierRectId.color

    //Set up the custom count property
    property int count: 0

    required property Receiver target

    onTargetChanged: {
        notify.connect(target.receiveInfo)
    }

    //Set up the signal
    signal notify(string count)

    Rectangle {
        id: notifierRectId
        width: 200
        height: 200
        color: "red"

        Text {
            id: displayTextId
            anchors.centerIn: parent
            font.pointSize: 20
            text: rootId.count
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                rootId.count++
                console.log("notifier: " + rootId.count)
                rootId.notify(rootId.count)
            }
        }
    }
}
