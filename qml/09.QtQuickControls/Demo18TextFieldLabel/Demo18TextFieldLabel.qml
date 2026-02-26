import QtQuick
import QtQuick.Controls

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("TextField and Label")

    Column {
        spacing: 30
        anchors.centerIn: parent

        Row {
            spacing: 30
            width: 300
            Label {
                width: 100
                height: 50
                wrapMode: Label.Wrap
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                text: "First name: "
            }

            TextField {
                id: textFieldId
                width: 200
                height: 50
                placeholderText: "Type in your name"
                onEditingFinished: function () {
                    console.log("Current text: " + text)
                }
            }
        }

        Button {
            text: "Click"
            onClicked: function () {
                console.log("Text: " + textFieldId.text)
            }
        }
    }
}
