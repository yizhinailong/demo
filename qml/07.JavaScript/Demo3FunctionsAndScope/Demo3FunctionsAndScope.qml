import QtQuick

Window {
    id: rootId
    width: 640
    height: 480
    visible: true
    title: qsTr("Functions and Scope")

    //Function in the global scope
    function min(a, b): real {
        return Math.min(a, b)
    }

    Rectangle {
        id: mRectId
        width: rootId.min(500, 400)
        height: 100
        anchors.centerIn: parent
        color: "blue"

        //Set up a function inside this rectangle
        function sayMessage() {
            console.log("Hello there.")
        }

        MouseArea {
            anchors.fill: parent
            onClicked: function () {
                mRectId.sayMessage()
            }
        }
    }

    Component.onCompleted: {
        console.log("The minimum is " + rootId.min(1000, 200))
        //mRectId.sayMessage() //Go through the Id to reference a nested name
    }
}
