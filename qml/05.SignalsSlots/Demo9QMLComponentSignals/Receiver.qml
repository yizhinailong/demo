import QtQuick

Item {
    width: receiverRectId.width
    height: receiverRectId.height

    property alias rectColor: receiverRectId.color

    function receiveInfo(count) {
        displayTextId.text = count
        console.log("Receiver: " + count)
    }

    Rectangle {
        id: receiverRectId
        width: 200
        height: 200
        color: "red"

        Text {
            id: displayTextId
            anchors.centerIn: parent
            font.pointSize: 20
            text: "0"
        }
    }
}
