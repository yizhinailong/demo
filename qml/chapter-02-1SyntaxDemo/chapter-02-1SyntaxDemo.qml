import QtQuick

Rectangle {
    id: root
    width: 640
    height: 480
    color: "green"

    Image {
        id: image
        source: "../res/pinwheel.png"
        x: (parent.width - image.width) / 2
        y: (root.height - image.height) / 2
    }

    Text {
        id: text
        text: qsTr("大风车。。。转呀转")
        color: "red"
        width: root.width
        horizontalAlignment: Text.AlignHCenter
        // x: (root.width - text.width) / 2
        y: image.y + image.height + 20
    }
}
