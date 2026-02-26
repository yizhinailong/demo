import QtQuick
import QtQuick.Controls

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Calling C++ from QML")

    Column {
        spacing: 10
        anchors.horizontalCenter: parent.horizontalCenter

        Row {
            Text {
                text: "RegisterMethod()"
            }
            Button {
                text: "Call C++ method"
                onClicked: function () {
                    CppWorker.RegisterMethod()
                }
            }
        }

        Row {
            Text {
                text: qsTr("CppSlot()")
            }

            Button {
                text: "Call C++ slot"
                onClicked: {
                    CppWorker.CppSlot()
                }
            }
        }

        Row {
            Text {
                id: returnTextId
                text: qsTr("return")
            }

            Text {
                text: "RegisterMethodWithReturn("
            }
            TextField {
                id: nameFieldId
                placeholderText: qsTr("name")
                text: qsTr("John")
            }
            Text {
                text: qsTr(",")
            }
            TextField {
                id: ageFieldId
                placeholderText: qsTr("age")
                inputMethodHints: Qt.ImhDigitsOnly
                text: qsTr("25")
            }
            Text {
                text: qsTr(")")
            }
            Button {
                text: qsTr("Call C++ method")
                onClicked: {
                    if (nameFieldId.text !== null && ageFieldId.text !== null) {
                        var response = CppWorker.RegisterMethodWithReturn(
                                    nameFieldId.text, parseInt(ageFieldId.text))
                        returnTextId.text = response
                    } else {
                        console.log("One of the two required fields is empty")
                    }
                }
            }
        }
    }
}
