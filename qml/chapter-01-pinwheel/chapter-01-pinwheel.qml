import QtQuick

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    Image {
        id: background
        source: "../res/background.png"

        anchors.fill: parent // 用当前的元素填充 parent

        Image {
            id: pole
            source: "../res/pole.png"

            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Image {
            id: pinWheel
            source: "../res/pinwheel.png"

            anchors.centerIn: parent

            // anchors.verticalCenter: parent.verticalCenter
            // anchors.horizontalCenter: parent.horizontalCenter
            Behavior on rotation {
                NumberAnimation {
                    duration: 500
                }
            }
        }

        MouseArea {
            anchors.fill: parent

            onClicked: pinWheel.rotation += 180
        }
    }
}
